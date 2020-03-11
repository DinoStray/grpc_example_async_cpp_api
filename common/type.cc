#include "type.h"

std::ostream& operator<<(std::ostream& os, GrpcEvent event) {
    // omit default case to trigger compiler warning for missing cases
    switch (event) {
        case GRPC_EVENT_CONNECTED:
            return os << "GRPC_EVENT_CONNECTED";
        case GRPC_EVENT_READ_DONE:
            return os << "GRPC_EVENT_READ_DONE";
        case GRPC_EVENT_WRITE_DONE:
            return os << "GRPC_EVENT_WRITE_DONE";
        case GRPC_EVENT_FINISHED:
            return os << "GRPC_EVENT_FINISHED";
        case GRPC_EVENT_MAX:
            return os << "GRPC_EVENT_MAX";
    }
}