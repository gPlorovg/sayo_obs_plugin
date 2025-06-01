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

    if (context_) context_->TryCancel();
    if (stream_) stream_->WritesDone();

    if (sender_thread_.joinable()) sender_thread_.join();
    if (receiver_thread_.joinable()) receiver_thread_.join();

    if (stream_) {
        grpc::Status status = stream_->Finish();
        obs_log(LOG_ERROR, "grpc_client: Finished with status: %s", status.error_message().c_str());
    }
}


void ASRGrpcClient::SendChunk(const std::vector<char>& chunk) {
    if (!stream_ || !running_) return;

    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        audio_queue_.push(chunk);
    }
    cv_.notify_one();
}

void ASRGrpcClient::SenderLoop() {

    while (running_ && stream_) {
        std::vector<char> chunk;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv_.wait(lock, [&] { return !audio_queue_.empty() || !running_; });
            if (!running_) break;
            chunk = std::move(audio_queue_.front());
            audio_queue_.pop();
        }
        sayo::AudioChunk msg;
        msg.set_pcm(reinterpret_cast<const char*>(chunk.data()), chunk.size());
        if (!stream_->Write(msg)) {
            obs_log(LOG_ERROR, "Failed to write audio chunk");
        }
    }
    obs_log(LOG_INFO, "SenderLoop: finished");
}

void ASRGrpcClient::ReceiverLoop() {
    sayo::ASRResult result;
    while (running_ && stream_) {
        if (!stream_->Read(&result)) {
            obs_log(LOG_INFO, "ReceiverLoop: stream_->Read returned false, exiting loop");
            break;
        }
        if (!running_) break;

        const std::string &text = result.text();
        if (!text.empty()) {
            std::lock_guard<std::mutex> lock(queue_mutex);
            asr_results_queue.push(text);
        }
    }
    obs_log(LOG_INFO, "ReceiverLoop: finished");
}

bool ASRGrpcClient::IsRunning() {
    return running_;
}

bool ASRGrpcClient::TestConnection() const {
    grpc::ClientContext ctx;

    auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(560 * 10);
    ctx.set_deadline(deadline);

    sayo::PingRequest req;
    sayo::PingResponse resp;
    grpc::Status status = stub_->Ping(&ctx, req, &resp);

    if (status.ok()) {
        obs_log(LOG_INFO, "gRPC Ping OK: %s", resp.message().c_str());
        return true;
    }
    obs_log(LOG_ERROR, "gRPC Ping FAILED: %s", status.error_message().c_str());
    return false;
}
