//
//  LoggerInterface.cpp
//  ee_x
//
//  Created by Zinge on 11/1/17.
//
//

#include "ee/core/Logger.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/Utils.hpp"

namespace ee {
namespace core {
using Self = Logger;

Self& Self::getSystemLogger() {
    static Self logger(
        "ee_x", [](const LogLevel& level, const std::string& tag,
                   const std::string& message) { log(level, tag, message); });
    return logger;
}

void Self::setSystemLogger(const Self& logger) {
    getSystemLogger() = logger;
}

Self Self::nullLogger() {
    return Self("", nullptr);
}

Self::Logger(const std::string& tag)
    : Logger(tag,
             [](const LogLevel& level, const std::string& tag,
                const std::string& message) { log(level, tag, message); }) {}

Self::Logger(const std::string& tag, const LogCallback& callback)
    : enabled_(true)
    , tag_(tag)
    , callback_(callback) {}

void Self::setEnabled(bool enabled) {
    enabled_ = enabled;
}

void Self::log(const LogLevel& level, std::string formatString, ...) {
    std::va_list args;
    va_start(args, formatString);
    log(level, formatString, args);
    va_end(args);
}

void Self::log(const LogLevel& level, std::string formatString,
               std::va_list args) {
    if (enabled_ && callback_) {
        auto message = format(formatString, args);
        callback_(level, tag_, message);
    }
}

void Self::verbose(std::string formatString, ...) {
    std::va_list args;
    va_start(args, formatString);
    log(LogLevel::Verbose, formatString, args);
    va_end(args);
}

void Self::debug(std::string formatString, ...) {
    std::va_list args;
    va_start(args, formatString);
    log(LogLevel::Debug, formatString, args);
    va_end(args);
}

void Self::info(std::string formatString, ...) {
    std::va_list args;
    va_start(args, formatString);
    log(LogLevel::Info, formatString, args);
    va_end(args);
}

void Self::warn(std::string formatString, ...) {
    std::va_list args;
    va_start(args, formatString);
    log(LogLevel::Warn, formatString, args);
    va_end(args);
}

void Self::error(std::string formatString, ...) {
    std::va_list args;
    va_start(args, formatString);
    log(LogLevel::Error, formatString, args);
    va_end(args);
}

void Self::wtf(std::string formatString, ...) {
    std::va_list args;
    va_start(args, formatString);
    log(LogLevel::Assert, formatString, args);
    va_end(args);
}
} // namespace core
} // namespace ee
