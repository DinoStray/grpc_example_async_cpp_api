#ifndef GRPC_EXAMPLE_TIME_SERVER_SESSION_H
#define GRPC_EXAMPLE_TIME_SERVER_SESSION_H

#include <greeting.grpc.pb.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/server_context.h>

#include <cstdint>
#include <deque>
#include <mutex>
#include <string>

#include "../common/type.h"

class GreetingSession {
  public:
    enum class SessionStatus { WAIT_CONNECT, READY_TO_WRITE, WAIT_WRITE_DONE, FINISHED };

  public:
    GreetingSession(uint64_t sessionId) : session_id_(sessionId){};

    bool init();

    void process(GrpcEvent event);

    void reply();

    void finish();

  public:
    const uint64_t session_id_{0};
    std::mutex mutex_{};
    SessionStatus status_{SessionStatus::WAIT_CONNECT};

    ::grpc::ServerContext server_context_{};
    ::grpc::ServerAsyncReaderWriter<::grpc::example::ReplyGreeting, ::grpc::example::RequestSubscribe>
        subscribe_stream{&server_context_};
    ::grpc::example::RequestSubscribe request_{};
    ::grpc::example::ReplyGreeting reply_{};

    std::string name_{};
    std::deque<std::shared_ptr<::grpc::example::ReplyGreeting>> message_queue_{};
};

std::ostream& operator<<(std::ostream& os, GreetingSession::SessionStatus sessionStatus);

#endif  // GRPC_EXAMPLE_TIME_SERVER_SESSION_H
