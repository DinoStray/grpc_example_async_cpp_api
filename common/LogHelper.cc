#include "LogHelper.h"

INITIALIZE_EASYLOGGINGPP

bool initLog(int argc, const char** argv, const std::string& logFileName) {
    // init log
    {
        START_EASYLOGGINGPP(argc, argv);
        std::string exe_file_name = "server";
        el::Configurations defaultConf;
        defaultConf.setToDefault();

        std::cout << "log_mode: ALL_INFO" << std::endl;
        defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        el::Loggers::setLoggingLevel(el::Level::Info);

        std::string log_file_name = "./" + exe_file_name + ".log";
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %level %fbase-%line] %msg");
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "10485760");  // 10MB
        defaultConf.setGlobally(el::ConfigurationType::Filename, log_file_name);

        el::Loggers::reconfigureLogger("default", defaultConf);
        el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
        el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
        el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    }
    return true;
}