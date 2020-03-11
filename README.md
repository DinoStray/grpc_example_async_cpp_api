# Grpc async bi-direction stream example

A simple example about grpc async C++ api

## Key words

* async C++ api
* bi-direction stream
* event base
* pubsub / subscriber
* show the difference between grpc semantic: "call completion queue" and "notification completion queue"
* graceful shutdown server and client

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

## Complain

The grpc C++ async api is terrible
* https://groups.google.com/forum/#!topic/grpc-io/7P8QvBBNq_E

And I hope the following feature will be accomplished as soon as possible
* https://github.com/grpc/grpc/issues/7352
