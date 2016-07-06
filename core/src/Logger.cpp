//
//  PluginUtils.cpp
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#include "Logger.hpp"
#include "LogLevel.hpp"

namespace ee {
namespace core {
Logger::Logger(const std::string& tag)
    : tag_{tag}
    , enabled_{true} {}

void Logger::setEnabled(bool enabled) noexcept { enabled_ = enabled; }

bool Logger::isEnabled() const noexcept { return enabled_; }

void Logger::log(const LogLevel& level, const std::string& message) const {
    if (isEnabled()) {
        log(level, tag_, message);
    }
}

void Logger::debug(const std::string& message) const {
    log(LogLevel::Debug, message);
}

void Logger::info(const std::string& message) const {
    log(LogLevel::Info, message);
}

void Logger::warn(const std::string& message) const {
    log(LogLevel::Warn, message);
}

void Logger::error(const std::string& message) const {
    log(LogLevel::Error, message);
}
} // namespace core
} // namespace ee