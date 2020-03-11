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

    try {
        if (!GreetingClient::getInstance().init("localhost:6666")) {
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
