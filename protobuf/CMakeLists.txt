message(STATUS "** sub directory protobuf")

find_package(Protobuf CONFIG REQUIRED)
message(STATUS "protobuf version: ${Protobuf_VERSION}")
find_program(_PROGRAM_PROTOC protoc)

find_package(gRPC CONFIG REQUIRED)
message(STATUS "grpc version: ${gRPC_VERSION}")
find_program(_PROGRAM_GRPC_CPP_PLUGIN grpc_cpp_plugin)

find_package(Threads REQUIRED)

execute_process(
        COMMAND ${_PROGRAM_PROTOC} -I=./ --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=${_PROGRAM_GRPC_CPP_PLUGIN} ./greeting.proto
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/protobuf
)

file(GLOB LIB_GENERATED_SOURCES ${PROJECT_SOURCE_DIR}/protobuf/*.cc)

add_library(grpc_greeting STATIC ${LIB_GENERATED_SOURCES})
target_link_libraries(grpc_greeting gRPC::grpc++_unsecure protobuf::libprotobuf)

message(STATUS "** sub directory protobuf done")