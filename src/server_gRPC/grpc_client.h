//
// Created by plorov on 5/30/25.
//

#ifndef GRPC_CLIENT_H
#define GRPC_CLIENT_H
#pragma once

#include "sayo.pb.h"
#include "sayo.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>

struct asr_source; // Forward declaration

class ASRGrpcClient {
public:
    ASRGrpcClient(const std::string& server, int port, asr_source* context);
    ~ASRGrpcClient();

    void Start();
    void Stop();
    void SendChunk(const std::vector<char>& chunk);
    bool IsRunning();
    [[nodiscard]] bool TestConnection() const;

    std::queue<std::string> asr_results_queue;
    std::mutex queue_mutex;

private:
    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<grpc::ClientContext> context_;
    std::unique_ptr<sayo::SayoService::Stub> stub_;
    std::unique_ptr<grpc::ClientReaderWriter<sayo::AudioChunk, sayo::ASRResult>> stream_;


    std::atomic<bool> running_{false};
    std::thread sender_thread_;
    std::thread receiver_thread_;
    std::condition_variable cv_;
    std::queue<std::vector<char>> audio_queue_;

    asr_source* ctx_;

    void SenderLoop();
    void ReceiverLoop();
};

#endif //GRPC_CLIENT_H
