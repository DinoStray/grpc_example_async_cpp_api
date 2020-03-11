#include "GreetingSession.h"

#include "GreetingServer.h"
#include "LogHelper.h"

bool GreetingSession::init() {
    GreetingServer::getInstance().greeting_async_service_.RequestSubscribeGreetingBySecond(
        &server_context_,
        &subscribe_stream,
        GreetingServer::getInstance().completion_queue_call_.get(),
        GreetingServer::getInstance().completion_queue_notification_.get(),
        reinterpret_cast<void*>(session_id_ << GRPC_EVENT_BIT_LENGTH | GRPC_EVENT_CONNECTED));
    return true;
}

void GreetingSession::process(GrpcEvent event) {
    LOG(INFO) << "session_id_: " << session_id_ << ", current status: " << status_ << ", event: " << event;
    switch (event) {
        case GRPC_EVENT_CONNECTED:
            subscribe_stream.Read(
                &request_,
                reinterpret_cast<void*>(session_id_ << GRPC_EVENT_BIT_LENGTH | GRPC_EVENT_READ_DONE));
            status_ = GrpcSessionStatus::READY_TO_WRITE;
            GreetingServer::getInstance().addSession();
            return;
        case GRPC_EVENT_READ_DONE:
            LOG(INFO) << "session_id_: " << session_id_ << ", new request: " << request_.ShortDebugString();
            name_ = request_.name();
            subscribe_stream.Read(
                &request_,
                reinterpret_cast<void*>(session_id_ << GRPC_EVENT_BIT_LENGTH | GRPC_EVENT_READ_DONE));
            reply();
            return;
        case GRPC_EVENT_WRITE_DONE:
            if (!message_queue_.empty()) {
                status_ = GrpcSessionStatus::WAIT_WRITE_DONE;
                subscribe_stream.Write(
                    *message_queue_.front(),
                    reinterpret_cast<void*>(session_id_ << GRPC_EVENT_BIT_LENGTH | GRPC_EVENT_WRITE_DONE));
                message_queue_.pop_front();
            } else {
                status_ = GrpcSessionStatus::READY_TO_WRITE;
            }
            return;
        default:
            LOG(INFO) << "session_id_: " << session_id_ << ", unhandled event: " << event;
            return;
    }
}

void GreetingSession::reply() {
    if (status_ != GrpcSessionStatus::READY_TO_WRITE && status_ != GrpcSessionStatus::WAIT_WRITE_DONE) {
        return;
    }
    auto new_message = std::make_shared<::grpc::example::ReplyGreeting>();
    new_message->set_message(name_ + ": " + std::to_string(time(nullptr)));
    if (status_ == GrpcSessionStatus::READY_TO_WRITE) {
        status_ = GrpcSessionStatus::WAIT_WRITE_DONE;
        subscribe_stream.Write(
            *new_message,
            reinterpret_cast<void*>(session_id_ << GRPC_EVENT_BIT_LENGTH | GRPC_EVENT_WRITE_DONE));
    } else {
        message_queue_.emplace_back(new_message);
    }
}

void GreetingSession::finish() {
    if (status_ == GrpcSessionStatus::WAIT_CONNECT) { return; }
    subscribe_stream.Finish(
        ::grpc::Status::CANCELLED,
        reinterpret_cast<void*>(session_id_ << GRPC_EVENT_BIT_LENGTH | GRPC_EVENT_FINISHED));
}