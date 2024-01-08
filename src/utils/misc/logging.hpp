#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include <vector>


namespace utils {

    /**
     * @brief Severity level of log messages.
     */
    enum LogLevel {
        ERROR = 0,
        WARN = 1,
        INFO = 2,
        TRACE = 3,
    };


    /**
     * @brief Logger output stream class. For internal logging framework use only.
     */
    class LogStream {
    private:
        std::string const& name;
        LogLevel const level;

        std::stringstream os;

    public:
        LogStream(std::string const& name, LogLevel const level) : name(name), level(level) {}

        /**
         * @brief Move constructor to allow returning of LogStream instances from
         * Logger utility methods.
         */
        LogStream(LogStream&& other) : name(other.name), level(other.level), os(std::move(other.os)) {}

        /**
         * @brief Destructor emits messages to output stream when object goes out of scope.
         */
        ~LogStream();

        /**
         * @brief Get the stream for logging operations.
         */
        std::stringstream& stream() {
            return os;
        }
    };


    /**
     * @brief Null logging stream. Skips stream formatting for minimal overhead.
     */
    class NullLogStream : public std::ostream {
    public:
        NullLogStream() : std::ostream(nullptr) {}

        template<typename T>
        friend NullLogStream& operator<<(NullLogStream& stream, T const& value) {
            return stream;
        }
    };


    /**
     * @brief Logger class, prefixes log messages with the name of the logger and the
     * severity level of the message.
     */
    class Logger {
    private:
        std::string const name;

    public:
        static LogLevel globalLoggingLevel;
        static NullLogStream nullLogStream;

        /**
         * @brief Construct new logger with the provided name.
         * Presents a stream interface and prepends name and log level to messages.
         * @param name Name of the logger to be prepended to log messages.
         */
        Logger(const std::string name) : name(name) {}

        /**
         * Create output streams for accepting messages at various logging levels.
         */
        LogStream log(LogLevel const level);
    };


    /**
     * @brief Overload for printing std::vector instances.
     * Not sure why this isn't default...
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& ss, std::vector<T> const& vector) {
        ss << '[';
        unsigned i = vector.size();
        for (auto const& element : vector) {
            ss << element << ((--i > 0) ? ", " : "]");
        }
        return ss;
    }


#ifndef NDEBUG
    #define TRACE(logger)   logger.log(utils::TRACE).stream()
    #define INFO(logger)    logger.log(utils::INFO).stream()
    #define WARN(logger)    logger.log(utils::WARN).stream()
    #define ERROR(logger)   logger.log(utils::ERROR).stream()
#else
    #define TRACE(logger)   logger.nullLogStream
    #define INFO(logger)    logger.nullLogStream
    #define WARN(logger)    logger.nullLogStream
    #define ERROR(logger)   logger.nullLogStream
#endif

}
