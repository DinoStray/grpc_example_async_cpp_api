#include "LogHelper.h"

INITIALIZE_EASYLOGGINGPP

bool initLog(int argc, const char** argv, const std::string& logFileName) {
    // init log
    {
        START_EASYLOGGINGPP(argc, argv);
        el::Configurations defaultConf;
        defaultConf.setToDefault();

        std::string log_level = "debug";
        const char* env_log_level = getenv("LOG_LEVEL");
        if (env_log_level != nullptr && strncmp(env_log_level, "", sizeof("")) != 0) {
            log_level = env_log_level;
        }

        std::cout << "LOG_LEVEL: " << log_level << std::endl;
        defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");

        if (log_level == "release") {
            el::Loggers::setLoggingLevel(el::Level::Info);
        } else {
            el::Loggers::setLoggingLevel(el::Level::Debug);
        }

        std::string full_log_file_name = "./" + logFileName + ".log";
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %level %fbase-%line] %msg");
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "10485760");  // 10MB
        defaultConf.setGlobally(el::ConfigurationType::Filename, full_log_file_name);

        el::Loggers::reconfigureLogger("default", defaultConf);
        el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
        el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
        el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    }
    return true;
}