#include "utils/misc/logging.hpp"

#include <vector>
#include <string>
#include <sstream>


namespace utils {

#ifdef NDEBUG
    LogLevel Logger::globalLoggingLevel = WARN;
#else
    LogLevel Logger::globalLoggingLevel = INFO;
#endif


    NullLogStream Logger::nullLogStream = NullLogStream();


    std::vector<std::string> const logLevelText = {
        "ERROR",
        "WARNING",
        "INFO",
        "TRACE"
    };


    LogStream::~LogStream() {
        if (level <= Logger::globalLoggingLevel) {
            std::cout << logLevelText[level] << " | " << name << " | " << os.str();
        }
    }


    LogStream Logger::log(LogLevel const level) {
        return LogStream(name, level);
    }
}
