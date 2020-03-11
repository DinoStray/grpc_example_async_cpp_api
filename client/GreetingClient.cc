#include "GreetingClient.h"

#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <thread>

#include "LogHelper.h"

bool GreetingClient::init(const std::string &server) {
    stub_ = ::grpc::example::GreetingService::NewStub(
        grpc::CreateChannel(server, grpc::InsecureChannelCredentials()));
    return true;
}

void GreetingClient::run() {
    running_ = true;
    stream_ = stub_->SubscribeGreetingBySecond(&context_);

    std::thread thread_for_reply{[this] {
        LOG(INFO) << "thread_for_reply run";
        ::grpc::example::ReplyGreeting reply;
        while (stream_->Read(&reply)) { LOG(INFO) << "receive reply: " << reply.ShortDebugString(); }
        LOG(INFO) << "thread_for_reply stop";
    }};

    std::thread thread_for_request{[this] {
        LOG(INFO) << "thread_for_request run";
        uint64_t increase_name{0};
        ::grpc::example::RequestSubscribe request;

        while (running_) {
            request.set_name(std::to_string(++increase_name));
            if (!stream_->Write(request)) {
                LOG(INFO) << "session closed";
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        LOG(INFO) << "thread_for_request stop";
    }};

    if (thread_for_reply.joinable()) { thread_for_reply.join(); }
    if (thread_for_request.joinable()) { thread_for_request.join(); }
}

void GreetingClient::stop() {
    if (!running_) { return; }
    running_ = false;
    LOG(INFO) << "context_->TryCancel() begin";
    context_.TryCancel();
    LOG(INFO) << "context_->TryCancel() end";
}
