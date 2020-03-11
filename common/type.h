#ifndef GRPC_EXAMPLE_TYPE_H
#define GRPC_EXAMPLE_TYPE_H
#include <iostream>

// not use enum class for easy compare
enum GrpcEvent {
    GRPC_EVENT_CONNECTED = 0,
    GRPC_EVENT_READ_DONE = 1,
    GRPC_EVENT_WRITE_DONE = 2,
    GRPC_EVENT_FINISHED = 3,
    GRPC_EVENT_MAX = 4
};

std::ostream& operator<<(std::ostream& os, GrpcEvent event);

#endif  // GRPC_EXAMPLE_TYPE_H
