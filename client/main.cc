#include <LogHelper.h>
#include <easylogging/easylogging++.h>

#include "GreetingClient.h"

void handler(int sig) {
    LOG(INFO) << "get signal: " << sig;
    GreetingClient::getInstance().stop();
}

int main(int argc, const char **argv) {
    signal(SIGTERM, handler);
    signal(SIGINT, handler);

    if (!initLog(argc, argv, "client")) {
        std::cout << "init log failed" << std::endl;
        return 1;
    }

    std::string client_name{"client_name"};
    if (argc >= 2) {
        client_name = argv[1];
        LOG(INFO) << "client name: " << client_name;
    }

    std::string server_address{"localhost:6666"};
    if (argc >= 3) {
        server_address = argv[2];
        LOG(INFO) << "server address: " << server_address;
    }

    try {
        if (!GreetingClient::getInstance().init(client_name, server_address)) {
            LOG(INFO) << "GreetingClient init failed";
            return 1;
        }
        GreetingClient::getInstance().run();
    } catch (std::exception &e) {
        LOG(INFO) << "exception: " << e.what();
        return 1;
    }

    return 0;
}
