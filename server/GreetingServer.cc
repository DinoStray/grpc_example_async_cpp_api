#include "GreetingServer.h"

#include <grpcpp/grpcpp.h>

#include <thread>

#include "LogHelper.h"

bool GreetingServer::init(const std::string &server) {
    ::grpc::ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server, grpc::InsecureServerCredentials());
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&greeting_async_service_);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    completion_queue_call_ = builder.AddCompletionQueue();
    completion_queue_notification_ = builder.AddCompletionQueue();
    // Finally assemble the server.
    server_ = builder.BuildAndStart();
    LOG(INFO) << "GreetingServer listening on: " << server;
    return true;
}

void GreetingServer::run() {
    running_ = true;
    std::thread thread_completion_queue_call([this] {
        // spawn the first rpc call session
        addSession();
        uint64_t session_id;  // uniquely identifies a request.
        bool ok;
        // rpc event "read done / write done / close(already connected)" call-back by this completion queue
        while (completion_queue_call_->Next(reinterpret_cast<void **>(&session_id), &ok)) {
            auto event = static_cast<GrpcEvent>(session_id & GRPC_EVENT_MASK);
            session_id = session_id >> GRPC_EVENT_BIT_LENGTH;
            LOG(INFO) << "session_id_: " << session_id << ", completion queue(call), event: " << event;
            auto session = getSession(session_id);
            if (session == nullptr) {
                LOG(INFO) << "session_id_: " << session_id << ", have been removed";
                continue;
            }
            if (!ok) {
                LOG(INFO) << "session_id_: " << session_id << ", rpc call closed";
                removeSession(session_id);
                continue;
            }
            {
                std::lock_guard<std::mutex> local_lock_guard{session->mutex_};
                session->process(event);
            }
        }
        LOG(INFO) << "completion queue(call) exit";
    });
    std::thread thread_completion_queue_notification([this] {
        uint64_t session_id;  // uniquely identifies a request.
        bool ok;
        // rpc event "new connection / close(waiting for connect)" call-back by this completion queue
        while (completion_queue_notification_->Next(reinterpret_cast<void **>(&session_id), &ok)) {
            auto event = static_cast<GrpcEvent>(session_id & GRPC_EVENT_MASK);
            session_id = session_id >> GRPC_EVENT_BIT_LENGTH;
            LOG(INFO) << "session_id_: " << session_id
                      << ", completion queue(notification), event: " << event;
            auto session = getSession(session_id);
            if (session == nullptr) {
                LOG(INFO) << "session_id_: " << session_id << ", have been removed";
                continue;
            }
            if (!ok) {
                LOG(INFO) << "session_id_: " << session_id << ", rpc call closed";
                removeSession(session_id);
                continue;
            }
            {
                std::lock_guard<std::mutex> local_lock_guard{session->mutex_};
                // deal request
                session->process(event);
            }
        }
        LOG(INFO) << "completion queue(notification) exit";
    });
    std::thread thread_publish_greeting([this] {
        while (running_) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            {
                std::lock_guard<std::mutex> local_lock_guard{mutex_sessions_};
                for (const auto &it : sessions_) { it.second->reply(); }
            }
        }
    });
    if (thread_completion_queue_call.joinable()) { thread_completion_queue_call.join(); }
    if (thread_completion_queue_notification.joinable()) { thread_completion_queue_notification.join(); }
    if (thread_publish_greeting.joinable()) { thread_publish_greeting.join(); }
    LOG(INFO) << "greeting server run() exit";
}

void GreetingServer::stop() {
    if (!running_) { return; }
    running_ = false;
    LOG(INFO) << "all sessions TryCancel() begin";
    // TODO: have to TryCancel(), otherwise the completion queue Next() not return if there is still connected
    // session(async reading or writing)
    {
        std::lock_guard<std::mutex> local_lock_guard{mutex_sessions_};
        for (const auto &it : sessions_) {
            if (it.second->status_ != GreetingSession::SessionStatus::WAIT_CONNECT) {
                it.second->server_context_.TryCancel();
            }
        }
    }
    LOG(INFO) << "server_->Shutdown() begin";
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    LOG(INFO) << "completion queue(call) Shutdown() begin";
    completion_queue_call_->Shutdown();
    LOG(INFO) << "completion queue(notification) Shutdown() begin";
    completion_queue_notification_->Shutdown();
    LOG(INFO) << "GreetingServer::stop() exit";
}

std::shared_ptr<GreetingSession> GreetingServer::addSession() {
    auto new_session_id = session_id_allocator_++;
    auto new_session = std::make_shared<GreetingSession>(new_session_id);
    if (!new_session->init()) {
        LOG(ERROR) << "init new session failed";
        return nullptr;
    }
    std::lock_guard<std::mutex> local_lock_guard{mutex_sessions_};
    sessions_[new_session_id] = new_session;
    LOG(INFO) << "session_id_: " << new_session_id << ", spawn new session and wait for connect";
    return new_session;
}

void GreetingServer::removeSession(uint64_t sessionId) {
    std::lock_guard<std::mutex> local_lock_guard{mutex_sessions_};
    sessions_.erase(sessionId);
}

std::shared_ptr<GreetingSession> GreetingServer::getSession(uint64_t sessionId) {
    std::lock_guard<std::mutex> local_lock_guard{mutex_sessions_};
    auto it = sessions_.find(sessionId);
    if (it == sessions_.end()) { return nullptr; }
    return it->second;
}
