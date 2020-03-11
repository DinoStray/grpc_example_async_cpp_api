#ifndef GRPC_EXAMPLE_TIME_SERVER_H
#define GRPC_EXAMPLE_TIME_SERVER_H

#include <greeting.grpc.pb.h>
#include <grpcpp/server.h>

#include "../common/type.h"
#include "GreetingSession.h"

class GreetingServer {
  public:
    GreetingServer(const GreetingServer &) = delete;

    GreetingServer(GreetingServer &&) = delete;

    GreetingServer &operator=(const GreetingServer &) = delete;

    GreetingServer &operator=(GreetingServer &&) = delete;

    // only for C++11, return static local var reference is thread safe
    static GreetingServer &getInstance() {
        static GreetingServer kSingleInstance;
        return kSingleInstance;
    }

    bool init(const std::string &server);

    void run();

    void stop();

    std::shared_ptr<GreetingSession> addSession();

    void removeSession(uint64_t sessionId);

    std::shared_ptr<GreetingSession> getSession(uint64_t sessionId);

  private:
    GreetingServer() = default;

    virtual ~GreetingServer() = default;

  public:
    std::atomic_bool running_{false};
    std::atomic_uint64_t session_id_allocator_{0};

    std::unique_ptr<::grpc::Server> server_{};
    ::grpc::example::GreetingService::AsyncService greeting_async_service_{};

    // subscribe_greeting
    std::unique_ptr<::grpc::ServerCompletionQueue> completion_queue_call_{};
    std::unique_ptr<::grpc::ServerCompletionQueue> completion_queue_notification_{};
    // better choice for production environment: boost::shared_mutex or std::shared_mutex(since C++17)
    std::mutex mutex_sessions_{};
    std::unordered_map<uint64_t /*session id*/, std::shared_ptr<GreetingSession>> sessions_{};
};

#endif  // GRPC_EXAMPLE_TIME_SERVER_H
