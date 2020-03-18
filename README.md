# Grpc async bi-direction stream example

A simple example about grpc async C++ api

## Key words

* async C++ api
* bi-direction stream
* state machine based event callback
* pubsub / subscriber
* shutdown server and client gracefully
* one service one completion queue(one thread)
* performance test

## Compile and run

```shell script
# complie
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
# run
## simple mode
./server
./client
## change log level, default debug
LOG_LEVEL=release ./server
LOG_LEVEL=release ./client
## specify client name
LOG_LEVEL=debug ./client client_name
## specify server address
LOG_LEVEL=release ./client client_name localhost:6666
```

## Test platform

* gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
* protobuf version: 3.11.2.0
* grpc version: 1.27.3