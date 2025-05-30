#include "grpc_client.h"
#include "sayo.grpc.pb.h"
#include <obs-module.h>
#include <plugin-support.h>

ASRGrpcClient::ASRGrpcClient(const std::string& address, asr_source* context)
    : ctx_(context)
{
    channel_ = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
    stub_ = sayo::SayoService::NewStub(channel_);
}

ASRGrpcClient::~ASRGrpcClient() {
    Stop();
}

void ASRGrpcClient::Start() {
    if (running_) return;
    running_ = true;
    context_ = std::make_unique<grpc::ClientContext>();
    stream_ = stub_->StreamingASR(context_.get());

    sender_thread_ = std::thread(&ASRGrpcClient::SenderLoop, this);
    receiver_thread_ = std::thread(&ASRGrpcClient::ReceiverLoop, this);
}


void ASRGrpcClient::Stop() {
    if (!running_) return;
    running_ = false;
    cv_.notify_all();
    if (sender_thread_.joinable()) sender_thread_.join();
    if (receiver_thread_.joinable()) receiver_thread_.join();

    if (stream_) {
        stream_->WritesDone();
        if (context_) context_->TryCancel();
        grpc::Status status = stream_->Finish();
        obs_log(LOG_ERROR, "grpc_client: Finished with status: %s", status.error_message().c_str());
    }
}


void ASRGrpcClient::SendChunk(const std::vector<float>& chunk) {
    if (!stream_ || !running_) return;

    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        audio_queue_.push(chunk);
    }
    cv_.notify_one();
}

void ASRGrpcClient::SenderLoop() {

    while (running_ && stream_) {
        std::vector<float> chunk;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv_.wait(lock, [&] { return !audio_queue_.empty() || !running_; });
            if (audio_queue_.empty() && !running_) break;
            chunk = std::move(audio_queue_.front());
            audio_queue_.pop();
        }
        sayo::AudioChunk msg;
        msg.set_pcm(reinterpret_cast<const char*>(chunk.data()), chunk.size() * sizeof(float));
        if (!stream_->Write(msg)) {
            obs_log(LOG_ERROR, "Failed to write audio chunk");
        }
    }
}

void ASRGrpcClient::ReceiverLoop() {
    sayo::ASRResult result;
    while (running_ && stream_ && stream_->Read(&result)) {
        const std::string &text = result.text();
        if (!text.empty()) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            asr_results_queue.push(text);
        }
    }
}

bool ASRGrpcClient::IsRunning() {
    return running_;
}
