#include <obs-module.h>
#include <plugin-support.h>
#include <string>
#include <atomic>
#include <vector>
#include <samplerate.h>
#include "server_gRPC/grpc_client.h"
#include "subtitle_buffer.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

static std::atomic<int> asr_instance_counter{0};

namespace asr_defaults {
	constexpr int TARGET_SAMPLE_RATE = 16000;
	constexpr int INPUT_SAMPLE_RATE = 48000;
	constexpr int RESAMPLER_WARMED_UP = 5;
	constexpr char SERVER_ADDRESS[] = "localhost";
	constexpr int SERVER_PORT = 50051;
	constexpr int MAX_LINES = 2;
	constexpr int MAX_CHARS_PER_LINE = 60;
}

struct asr_source {
	obs_source_t *source = nullptr;
	std::string selected_audio_source;
	obs_source_t *internal_text_source = nullptr;
	SRC_STATE *resampler = nullptr;
	uint32_t target_sample_rate = asr_defaults::TARGET_SAMPLE_RATE;
	uint32_t input_sample_rate = asr_defaults::INPUT_SAMPLE_RATE;

	float resample_ratio = static_cast<float>(target_sample_rate) / static_cast<float>(input_sample_rate);

	std::vector<float> resample_input_buffer;
	std::vector<float> resample_output_buffer;

	size_t audio_chunk_size = (target_sample_rate / 1000 * 160) * sizeof(float);
	std::vector<char> send_buffer;

	int resampler_warmed_up = asr_defaults::RESAMPLER_WARMED_UP;

	ASRGrpcClient* grpc_client = nullptr;
	std::string connect_status = "Unknown"; // Successful, Failed, Unknown, Connecting

	std::string server_address = asr_defaults::SERVER_ADDRESS;
	int server_port = asr_defaults::SERVER_PORT;
	int max_lines = asr_defaults::MAX_LINES;
	int max_chars_per_line = asr_defaults::MAX_CHARS_PER_LINE;

	SubtitlesBuffer* subtitles_buffer = nullptr;
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

bool is_empty_chunk(const float* buffer) {
	static constexpr float zeros[1200] = {0.0f};
	return memcmp(buffer, zeros, 1200 * sizeof(float)) == 0;
}

void audio_callback(void *param, [[maybe_unused]] obs_source_t *source, const struct audio_data *audio_data, bool muted)
{
	auto *ctx = static_cast<asr_source *>(param);

	if (!ctx || !ctx->grpc_client || !ctx->grpc_client->IsRunning()) return;
	if (muted) return;

	if (audio_data->frames == 1200 and is_empty_chunk(reinterpret_cast<float*>(audio_data->data[0]))) return;

	const size_t frames = audio_data->frames;
	std::vector<float> mono(frames);

	const auto *left = reinterpret_cast<float*>(audio_data->data[0]);
	if (audio_data->data[1]) {
		const auto *right = reinterpret_cast<const float *>(audio_data->data[1]);
		for (size_t i = 0; i < frames; ++i) {
			mono[i] = (left[i] + right[i]) * 0.5f;
		}
	} else {
		std::memcpy(mono.data(), left,  frames * sizeof(float));
	}
	const size_t out_frames = resample_audio(ctx, mono.data(), frames);

	if (ctx->resampler_warmed_up != 0) {
		ctx->resampler_warmed_up--;
		return;
	}

	const auto data = reinterpret_cast<const char *>(ctx->resample_output_buffer.data());

	ctx->send_buffer.insert(
		ctx->send_buffer.end(),
		data,
		data + out_frames * sizeof(float)
	);

	while (ctx->send_buffer.size() >= ctx->audio_chunk_size) {
		std::vector<char> chunk(ctx->send_buffer.begin(), ctx->send_buffer.begin() + ctx->audio_chunk_size);
		ctx->send_buffer.erase(ctx->send_buffer.begin(), ctx->send_buffer.begin() + ctx->audio_chunk_size);

		ctx->grpc_client->SendChunk(chunk);
	}
}

static void asr_update(void *data, obs_data_t *settings)
{
	auto *ctx = static_cast<asr_source *>(data);

	ctx->server_address = static_cast<std::string>(obs_data_get_string(settings, "server_address"));
	ctx->server_port = static_cast<int>(obs_data_get_int(settings, "server_port"));

	ctx->max_lines = static_cast<int>(obs_data_get_int(settings, "max_lines"));
	ctx->max_chars_per_line = static_cast<int>(obs_data_get_int(settings, "max_chars_per_line"));

	// Update audio source
	const char *audio_name = obs_data_get_string(settings, "audio_source");
	if (ctx->selected_audio_source.empty() || (ctx->selected_audio_source != audio_name)) {
		ctx->selected_audio_source = audio_name;
		obs_log(LOG_INFO, "Audio source set up: %s", audio_name);
	}
	// Update text box
	if (ctx->internal_text_source)
		obs_source_update(ctx->internal_text_source, settings);
}

static void *asr_create([[maybe_unused]] obs_data_t *settings, obs_source_t *source)
{
	auto *ctx = new asr_source;
	ctx->source = source;

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

	// Create resampler
	if (const audio_output_info *info = audio_output_get_info(obs_get_audio())) {
		ctx->input_sample_rate = info->samples_per_sec;
		obs_log(LOG_INFO, "Input sample rate from OBS: %d", ctx->input_sample_rate);
	} else {
		obs_log(LOG_ERROR, "Failed to get OBS audio sample rate. Using default 48000 → 16000");
	}
	ctx->resample_ratio = static_cast<float>(ctx->target_sample_rate) / static_cast<float>(ctx->input_sample_rate);
	obs_log(LOG_INFO, "Resample ratio: %.6f", ctx->resample_ratio);

	int err;
	ctx->resampler = src_new(SRC_SINC_FASTEST, 1, &err);
	if (!ctx->resampler) {
		obs_log(LOG_ERROR, "Failed to create resampler: %s", src_strerror(err));
	} else {
		obs_log(LOG_INFO, "Resampler created!");
	}

	// Get server parameters from settings
	ctx->server_address = obs_data_get_string(settings, "server_address");
	ctx->server_port = static_cast<int>(obs_data_get_int(settings, "server_port"));
	// Get text box parameters from settings
	ctx->max_lines = static_cast<int>(obs_data_get_int(settings, "max_lines"));
	ctx->max_chars_per_line = static_cast<int>(obs_data_get_int(settings, "max_chars_per_line"));

	// Prepare audio buffer
	ctx->send_buffer.reserve(ctx->audio_chunk_size);

	// Create subtitle buffer
	ctx->subtitles_buffer = new SubtitlesBuffer(ctx->max_lines, ctx->max_chars_per_line);
	return ctx;
}

static void asr_destroy(void *data)
{
	auto *ctx = static_cast<asr_source *>(data);

	if (!ctx->selected_audio_source.empty()) {
		if (obs_source_t *audio_src = obs_get_source_by_name(ctx->selected_audio_source.c_str())) {
			obs_source_remove_audio_capture_callback(audio_src, audio_callback, ctx);
			obs_log(LOG_INFO, "Audio callback destroyed!");
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

	delete ctx->subtitles_buffer;
	delete ctx;
}

struct UpdateUIArgs {
	asr_source* ctx;
	bool connected;
};
struct UpdateAudioCallbackArgs {
	asr_source* ctx;
	bool connected;
};
static void update_ui(void* param) {
	const auto args = static_cast<UpdateUIArgs*>(param);
	if (!args->connected) {
		obs_log(LOG_INFO, "Connection status: Failed!");
		args->ctx->connect_status = "Failed";
	} else {
		obs_log(LOG_INFO, "Connection status: Successful!");
		args->ctx->connect_status = "Successful";
	}
	delete args;
}
static void update_audio_callback(void* param) {
	const auto args = static_cast<UpdateAudioCallbackArgs*>(param);
	const auto ctx = args->ctx;

	if (!args->connected) {
		delete ctx->grpc_client;
		ctx->grpc_client = nullptr;
		obs_log(LOG_INFO, "No audio callback set up!");
	} else {
		if (ctx->grpc_client && !ctx->grpc_client->IsRunning())
			ctx->grpc_client->Start();

		if (obs_source_t *audio_src = obs_get_source_by_name(ctx->selected_audio_source.c_str())) {
			obs_source_remove_audio_capture_callback(audio_src, audio_callback, ctx);
			obs_log(LOG_INFO, "Previous audio callback destroyed");
			obs_source_add_audio_capture_callback(audio_src, audio_callback, ctx);
			obs_log(LOG_INFO, "New audio callback set up");
			obs_source_release(audio_src);
		} else {
			obs_log(LOG_ERROR, "Failed to set up audio callback!");
		}
	}
	delete args;
}

bool on_check_button_clicked(obs_properties_t* props, obs_property_t* property, void* data)
{
	auto *ctx = static_cast<asr_source *>(data);
	if (ctx->connect_status != "Connecting") {
		obs_property_t* status_prop = obs_properties_get(props, "connection_status");
		obs_property_set_description(status_prop, ("Connection status: " + ctx->connect_status).c_str());
		obs_property_set_enabled(obs_properties_get(props, "connect_button"), true);
		obs_property_set_enabled(obs_properties_get(props, "server_address"), true);
		obs_property_set_enabled(obs_properties_get(props, "server_port"), true);

		obs_property_set_enabled(property, false);
	}
	return true;
}

bool on_connect_button_clicked(obs_properties_t* props, obs_property_t* property, void* data)
{
	auto *ctx = static_cast<asr_source *>(data);
	ctx->connect_status = "Connecting";
	obs_property_t* status_prop = obs_properties_get(props, "connection_status");
	obs_property_set_enabled(property, false);
	obs_property_set_enabled(obs_properties_get(props, "server_address"), false);
	obs_property_set_enabled(obs_properties_get(props, "server_port"), false);
	obs_property_set_enabled(obs_properties_get(props, "check_button"), false);
	obs_property_set_description(status_prop, ("Connection status: " + ctx->connect_status + " Waiting 1-20s" + "...").c_str());

	obs_property_set_enabled(obs_properties_get(props, "check_button"), true);

	std::thread([ctx]() {

		auto update_ui_args = new UpdateUIArgs;
		auto update_audio_callback_args = new UpdateAudioCallbackArgs;

		int try_attempts = 3;
		bool connected = false;

		ctx->grpc_client = new ASRGrpcClient(ctx->server_address, ctx->server_port, ctx);

		while (try_attempts-- > 0) {
			if (!ctx || !ctx->grpc_client) break;
			if (ctx->grpc_client->TestConnection()) {
				connected = true;
				break;
			}
			obs_log(LOG_ERROR, "Failed to connect to gRPC server (%s:%d)!\n Attempts left: %d", ctx->server_address, ctx->server_port, try_attempts);
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}

		if (!ctx) return;

		update_audio_callback_args->connected = connected;
		update_audio_callback_args->ctx = ctx;
		obs_queue_task(OBS_TASK_UI, update_audio_callback, update_audio_callback_args, false);

		update_ui_args->connected = connected;
		update_ui_args->ctx = ctx;
		obs_queue_task(OBS_TASK_UI, update_ui, update_ui_args, true);
	}).detach();
	return true;
}


static obs_properties_t *asr_get_properties(void *data)
{
	auto *ctx = static_cast<asr_source *>(data);
	obs_properties_t *props = obs_properties_create();

	obs_property_t *list = obs_properties_add_list(
		props, "audio_source", "Audio Source",
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING
	);

	const auto server_address = obs_properties_add_text(props, "server_address", "Server address", OBS_TEXT_DEFAULT);
	const auto server_port = obs_properties_add_int(props, "server_port", "Port", 1, 65535, 1);

	const auto conn_btn = obs_properties_add_button(
		props,
		"connect_button",
		"Connect to server",
		on_connect_button_clicked
	);
	const auto conn_status = obs_properties_add_text(props, "connection_status", ("Connection status: " + ctx->connect_status).c_str(), OBS_TEXT_INFO);
	const auto check_btn = obs_properties_add_button(
		props,
		"check_button",
		"Check connection",
		on_check_button_clicked
	);
	if (ctx->connect_status != "Connecting") {
		obs_property_set_enabled(check_btn, false);
		obs_property_set_enabled(conn_btn, true);
		obs_property_set_enabled(server_address, true);
		obs_property_set_enabled(server_port, true);
	} else {
		obs_property_set_enabled(check_btn, true);
		obs_property_set_enabled(conn_btn, false);
		obs_property_set_enabled(server_address, false);
		obs_property_set_enabled(server_port, false);
		obs_property_set_description(conn_status, ("Connection status: " + ctx->connect_status + " Waiting 1-20s" + "...").c_str());
	}

	obs_properties_add_int(props, "max_lines", "Max lines", 1, 10, 1);
	obs_properties_add_int(props, "max_chars_per_line", "Max chars per line", 16, 100, 1);

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
	if (!ctx || !ctx->grpc_client || !ctx->grpc_client->IsRunning()) return;
	std::string asr_result;
	{
		std::lock_guard<std::mutex> lock(ctx->grpc_client->queue_mutex);
		if (!ctx->grpc_client->asr_results_queue.empty()) {
			asr_result = ctx->grpc_client->asr_results_queue.front();
			ctx->grpc_client->asr_results_queue.pop();
		}
	}
	if (!asr_result.empty() && ctx->internal_text_source) {
		ctx->subtitles_buffer->addWord(asr_result);
		obs_data_t *settings = obs_source_get_settings(ctx->internal_text_source);
		obs_data_set_string(settings, "text", ctx->subtitles_buffer->getBufferContent().c_str());
		obs_source_update(ctx->internal_text_source, settings);
		obs_data_release(settings);
	}
}

void asr_get_defaults(obs_data_t *settings) {
	obs_data_set_default_string(settings, "server_address", asr_defaults::SERVER_ADDRESS);
	obs_data_set_default_int(settings, "server_port", asr_defaults::SERVER_PORT);
	obs_data_set_default_int(settings, "max_lines", asr_defaults::MAX_LINES);
	obs_data_set_default_int(settings, "max_chars_per_line", asr_defaults::MAX_CHARS_PER_LINE);
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
	/* get_defaults */          asr_get_defaults,
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
