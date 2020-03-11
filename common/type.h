#ifndef GRPC_EXAMPLE_TYPE_H
#define GRPC_EXAMPLE_TYPE_H
#include <iostream>

#define GRPC_EVENT_MASK 0x3u
#define GRPC_EVENT_BIT_LENGTH 2u

enum GrpcEvent {
    GRPC_EVENT_CONNECTED = 0,
    GRPC_EVENT_READ_DONE = 1,
    GRPC_EVENT_WRITE_DONE = 2,
    GRPC_EVENT_FINISHED = 3
};
std::ostream& operator<<(std::ostream& os, GrpcEvent event);

enum class GrpcSessionStatus { WAIT_CONNECT, READY_TO_WRITE, WAIT_WRITE_DONE, FINISHED };
std::ostream& operator<<(std::ostream& os, GrpcSessionStatus sessionStatus);

#endif  // GRPC_EXAMPLE_TYPE_H
