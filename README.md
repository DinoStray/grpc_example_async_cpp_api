# Grpc async bi-direction stream example

A simple example about grpc async C++ api

## Key words

* async C++ api
* bi-direction stream
* state machine based event callback
* pubsub / subscriber
* shutdown server and client gracefully
* one service one completion queue(one thread)

## Compile and run

```shell script
# complie
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
# run
./server
./client
```

## Test platform

* gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
* protobuf version: 3.11.2.0
* grpc version: 1.27.3

## Discussion

I do not like the grpc C++ async api
* https://groups.google.com/forum/#!topic/grpc-io/7P8QvBBNq_E

Hope the following feature will be accomplished as soon as possible
* https://github.com/grpc/grpc/issues/7352
