#include "GreetingServer.h"
#include "LogHelper.h"

void handler(int sig) {
    std::cout << "get signal: " << sig << std::endl;
    GreetingServer::getInstance().stop();
}

int main(int argc, const char **argv) {
    signal(SIGTERM, handler);
    signal(SIGINT, handler);

    if (!initLog(argc, argv, "server")) {
        std::cout << "init log failed" << std::endl;
        return 1;
    }

    try {
        if (!GreetingServer::getInstance().init("0.0.0.0:6666")) {
            LOG(INFO) << "init failure";
            return 1;
        }
        GreetingServer::getInstance().run();
    } catch (std::exception &e) {
        LOG(INFO) << "exception: " << e.what();
        return 1;
    }

    return 0;
}
