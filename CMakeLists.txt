cmake_minimum_required(VERSION 3.15)

project(grpc_example VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo .. (default for docker release build)
message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

add_subdirectory(${PROJECT_SOURCE_DIR}/protobuf)

include_directories(
        ${PROJECT_SOURCE_DIR}/libs
        ${PROJECT_SOURCE_DIR}/protobuf
        ${PROJECT_SOURCE_DIR}/common)

# init easylogging
add_definitions(-DELPP_THREAD_SAFE)
add_library(easylogging STATIC ${PROJECT_SOURCE_DIR}/libs/easylogging/easylogging++.cc)

set(THIRD_LIBRARIES grpc_greeting easylogging)

file(GLOB_RECURSE SERVER_SOURCES common/*.cc ${PROJECT_SOURCE_DIR}/server/*.cc)
message("SERVER_SOURCES = ${SERVER_SOURCES}")
add_executable(server ${SERVER_SOURCES})
target_link_libraries(server ${THIRD_LIBRARIES})

file(GLOB_RECURSE CLIENT_SOURCES common/*.cc ${PROJECT_SOURCE_DIR}/client/*.cc)
message("CLIENT_SOURCES = ${CLIENT_SOURCES}")
add_executable(client ${CLIENT_SOURCES})
target_link_libraries(client ${THIRD_LIBRARIES})