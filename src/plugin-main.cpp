#include <obs-module.h>
#include <plugin-support.h>
#include <string>
#include <atomic>
#include <vector>
#include <samplerate.h>
#include "server_gRPC/grpc_client.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

static std::atomic<int> asr_instance_counter{0};

struct asr_source {
	std::string selected_audio_source;
	obs_source_t *internal_text_source = nullptr;
	SRC_STATE *resampler = nullptr;

	int target_sample_rate = 16000;
	uint32_t input_sample_rate = 48000;  // default value

	float resample_ratio = static_cast<float>(target_sample_rate) / static_cast<float>(input_sample_rate);

	std::vector<float> resample_input_buffer;
	std::vector<float> resample_output_buffer;

	int audio_chunk_size = target_sample_rate / 1000 * 160;
	std::vector<float> send_buffer;

	int resampler_warmed_up = 5;

	ASRGrpcClient* grpc_client = nullptr;
};

static const char *asr_get_name([[maybe_unused]] void *unused)
{
	return "ASR Text Source";
}

size_t resample_audio(asr_source *ctx, const float *in, size_t in_frames)
{
	size_t max_out_frames = static_cast<size_t>(static_cast<float>(in_frames) * ctx->resample_ratio) + 1;
	ctx->resample_output_buffer.resize(max_out_frames);

	SRC_DATA src_data;
	src_data.data_in = in;
	src_data.input_frames = static_cast<long>(in_frames);
	src_data.data_out = ctx->resample_output_buffer.data();
	src_data.output_frames = static_cast<long>(max_out_frames);
	src_data.src_ratio = ctx->resample_ratio;
	src_data.end_of_input = 0;


	if (int err = src_process(ctx->resampler, &src_data) != 0) {
		obs_log(LOG_ERROR, "Resample error: %s", src_strerror(err));
		return 0;
	}

	return src_data.output_frames_gen;
}

void audio_callback(void *param, [[maybe_unused]] obs_source_t *source, const struct audio_data *audio_data, bool muted)
{
	auto *ctx = static_cast<asr_source *>(param);
	if (!ctx || !ctx->grpc_client || !ctx->grpc_client->IsRunning()) return;

	if (muted)
		return;

	auto *samples = reinterpret_cast<const float *>(audio_data->data[0]);
	const size_t frames = audio_data->frames;

	const size_t out_frames = resample_audio(ctx, samples, frames);

	if (ctx->resampler_warmed_up != 0) {
		ctx->resampler_warmed_up--;
		return;
	}

	ctx->send_buffer.insert(
		ctx->send_buffer.end(),
		ctx->resample_output_buffer.begin(),
		ctx->resample_output_buffer.begin() + static_cast<std::vector<float>::difference_type>(out_frames)
	);

	while (ctx->send_buffer.size() >= 2560) {
		std::vector<float> chunk(ctx->send_buffer.begin(), ctx->send_buffer.begin() + 2560);
		ctx->send_buffer.erase(ctx->send_buffer.begin(), ctx->send_buffer.begin() + 2560);

		ctx->grpc_client->SendChunk(chunk);
	}
}

static void asr_update(void *data, obs_data_t *settings)
{
	auto *ctx = static_cast<asr_source *>(data);

	if (!ctx->grpc_client) {
		ctx->grpc_client = new ASRGrpcClient("localhost:50051", ctx);
		if (!ctx->grpc_client->TestConnection()) {
			obs_log(LOG_ERROR, "Unable to connect to gRPC server!");
			delete ctx->grpc_client;
			ctx->grpc_client = nullptr;
			return;
		}
	}

	if (ctx->grpc_client && !ctx->grpc_client->IsRunning()) ctx->grpc_client->Start();

	const char *audio_name = obs_data_get_string(settings, "audio_source");
	if (audio_name)
		ctx->selected_audio_source = audio_name;

	if (ctx->internal_text_source)
		obs_source_update(ctx->internal_text_source, settings);

	obs_log(LOG_INFO, "Audio source set up: %s", audio_name);

	if (obs_source_t *audio_src = obs_get_source_by_name(ctx->selected_audio_source.c_str())) {
		obs_source_add_audio_capture_callback(audio_src, audio_callback, ctx);
		obs_log(LOG_INFO, "Audio callback set up");
		obs_source_release(audio_src);
	} else {
		obs_log(LOG_ERROR, "Failed to set up audio callback!");
		obs_source_release(audio_src);
	}
}

static void *asr_create([[maybe_unused]] obs_data_t *settings, [[maybe_unused]] obs_source_t *source)
{
	auto *ctx = new asr_source;

	// Create internal text_ft2_source with passed-in properties
	obs_data_t *text_settings = obs_data_create();
	obs_data_set_string(text_settings, "text", "ASR Subtitles");

	std::string name;
	obs_source_t *src = nullptr;
	do {
		name = "__asr_internal_text_" + std::to_string(asr_instance_counter++);
		if (src) obs_source_release(src);
		src = obs_get_source_by_name(name.c_str());
	} while (src);

	ctx->internal_text_source = obs_source_create("text_ft2_source", name.c_str(), text_settings, nullptr);
	obs_data_release(text_settings);
	obs_log(LOG_INFO, "Internal text_ft2_source created with name: %s", name.c_str());

	if (const audio_output_info *info = audio_output_get_info(obs_get_audio())) {
		ctx->input_sample_rate = info->samples_per_sec;
		ctx->resample_ratio = static_cast<float>(ctx->target_sample_rate) / static_cast<float>(ctx->input_sample_rate);

		obs_log(LOG_INFO, "Input sample rate from OBS: %d", ctx->input_sample_rate);
		obs_log(LOG_INFO, "Resample ratio: %.6f", ctx->resample_ratio);
	} else {
		obs_log(LOG_ERROR, "Failed to get OBS audio sample rate; using default 48000 → 16000");
	}

	int err;
	ctx->resampler = src_new(SRC_SINC_FASTEST, 1, &err);
	if (!ctx->resampler) {
		obs_log(LOG_ERROR, "Failed to create resampler: %s", src_strerror(err));
	} else {
		obs_log(LOG_INFO, "Resampler created");
	}

	ctx->send_buffer.reserve(ctx->audio_chunk_size);

	asr_update(ctx, settings);
	return ctx;
}

static void asr_destroy(void *data)
{
	auto *ctx = static_cast<asr_source *>(data);

	if (!ctx->selected_audio_source.empty()) {
		if (obs_source_t *audio_src = obs_get_source_by_name(ctx->selected_audio_source.c_str())) {
			obs_source_remove_audio_capture_callback(audio_src, audio_callback, ctx);
			obs_log(LOG_INFO, "audio callback destroyed");
			obs_log(LOG_INFO, "remove audio callback: source=%p, ctx=%p", audio_src, ctx);
			obs_source_release(audio_src);
		}
	}
	if (ctx->grpc_client) {
		delete ctx->grpc_client;
		ctx->grpc_client = nullptr;
	}
	if (ctx->internal_text_source)
		obs_source_release(ctx->internal_text_source);
	if (ctx->resampler)
		src_delete(ctx->resampler);
	delete ctx;
}

static obs_properties_t *asr_get_properties([[maybe_unused]] void *data)
{
	obs_properties_t *props = obs_properties_create();

	obs_property_t *list = obs_properties_add_list(
		props, "audio_source", "Audio Source",
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING
	);

	obs_enum_sources([](void *data, obs_source_t *source) {
		if (obs_source_get_output_flags(source) & OBS_SOURCE_AUDIO) {
			const char *name = obs_source_get_name(source);
			obs_property_list_add_string(static_cast<obs_property_t *>(data), name, name);
		}
		return true;
	}, list);

	// Proxy properties from text_ft2_source
	obs_data_t *dummy_settings = obs_data_create();
	obs_source_t *dummy = obs_source_create_private("text_ft2_source", nullptr, dummy_settings);
	obs_data_release(dummy_settings);

	if (dummy) {
		obs_properties_t *text_props = obs_source_properties(dummy);

		for (obs_property_t *p = obs_properties_first(text_props); p; ) {
			const char *name = obs_property_name(p);
			const char *desc = obs_property_description(p);

			// Exclude text_ft2_source properties
			if (strcmp(name, "from_file") == 0 ||
				strcmp(name, "text_file") == 0 ||
				strcmp(name, "text") == 0)
			{
				obs_property_next(&p);
				continue;
			}

			switch (obs_property_get_type(p)) {
				case OBS_PROPERTY_FONT:
					obs_properties_add_font(props, name, desc);
					break;
				case OBS_PROPERTY_COLOR:
					obs_properties_add_color(props, name, desc);
					break;
				case OBS_PROPERTY_BOOL:
					obs_properties_add_bool(props, name, desc);
					break;
				case OBS_PROPERTY_INT:
					obs_properties_add_int(props, name, desc,
						obs_property_int_min(p), obs_property_int_max(p), obs_property_int_step(p));
					break;
				case OBS_PROPERTY_FLOAT:
					obs_properties_add_float(props, name, desc,
						obs_property_float_min(p), obs_property_float_max(p), obs_property_float_step(p));
					break;
				case OBS_PROPERTY_LIST:
					obs_properties_add_list(props, name, desc,
						obs_property_list_type(p), obs_property_list_format(p));
					break;
				default:
					break;
			}

			obs_property_next(&p);
		}

		obs_source_release(dummy);
	}

	return props;
}

static void asr_render(void *data,[[maybe_unused]] gs_effect_t *effect)
{
	auto *ctx = static_cast<asr_source *>(data);
	if (ctx->internal_text_source)
		obs_source_video_render(ctx->internal_text_source);
}

static uint32_t asr_get_width(void *data) {
	auto *ctx = static_cast<asr_source *>(data);
	return ctx->internal_text_source
		? obs_source_get_width(ctx->internal_text_source)
		: 0;
}

static uint32_t asr_get_height(void *data) {
	auto *ctx = static_cast<asr_source *>(data);
	return ctx->internal_text_source
		? obs_source_get_height(ctx->internal_text_source)
		: 0;
}

void asr_tick_callback(void *data, [[maybe_unused]] float seconds) {
	auto *ctx = static_cast<asr_source *>(data);
	if (ctx && ctx->grpc_client && ctx->grpc_client->IsRunning()) {
		std::string asr_result;
		{
			std::lock_guard<std::mutex> lock(ctx->grpc_client->queue_mutex);
			if (!ctx->grpc_client->asr_results_queue.empty()) {
				asr_result = ctx->grpc_client->asr_results_queue.front();
				ctx->grpc_client->asr_results_queue.pop();
			}
		}
		if (!asr_result.empty() && ctx->internal_text_source) {
			obs_data_t *settings = obs_source_get_settings(ctx->internal_text_source);
			obs_data_set_string(settings, "text", asr_result.c_str());
			obs_source_update(ctx->internal_text_source, settings);
			obs_data_release(settings);
		}
	}
}


static struct obs_source_info asr_source_info = {
	/* Required */
	/* id */                    "asr_text_source",
	/* type */                  OBS_SOURCE_TYPE_INPUT,
	/* output_flags */          OBS_SOURCE_VIDEO,

	/* get_name */              asr_get_name,
	/* create */                asr_create,
	/* destroy */               asr_destroy,
	/* get_width */             asr_get_width,
	/* get_height */            asr_get_height,

	/* Optional */
	/* get_defaults */          nullptr,
	/* get_properties */        asr_get_properties,
	/* update */                asr_update,
	/* activate */              nullptr,
	/* deactivate */            nullptr,
	/* show */                  nullptr,
	/* hide */                  nullptr,
	/* video_tick */            asr_tick_callback,
	/* video_render */          asr_render
};


extern "C" {

	const char *obs_module_name(void)
	{
		return "ASR Text Source";
	}

	const char *obs_module_description(void)
	{
		return "A text source that automatically transcribes audio into subtitles using NVIDIA Riva.";
	}

	bool obs_module_load(void)
	{
		obs_register_source(&asr_source_info);
		obs_log(LOG_INFO, "plugin loaded successfully (version %s)", PLUGIN_VERSION);
		return true;
	}

	void obs_module_unload(void)
	{
		obs_log(LOG_INFO, "plugin unloaded");
	}
}
