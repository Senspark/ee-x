//
//  PluginUtils.cpp
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#include "ee/core/Logger.hpp"
#include "ee/core/LogLevel.hpp"

namespace ee {
namespace core {
Logger::Logger(const std::string& tag)
    : tag_{tag}
    , enabled_{true} {
}

void Logger::setEnabled(bool enabled) noexcept {
    enabled_ = enabled;
}

bool Logger::isEnabled() const noexcept {
    return enabled_;
}

void Logger::log0(const LogLevel& level, const std::string& message) const {
    log0(level, tag_, message);
}

void Logger::debug0(const std::string& message) const {
    log0(LogLevel::Debug, message);
}

void Logger::info0(const std::string& message) const {
    log0(LogLevel::Info, message);
}

void Logger::warn0(const std::string& message) const {
    log0(LogLevel::Warn, message);
}

void Logger::error0(const std::string& message) const {
    log0(LogLevel::Error, message);
}
} // namespace core
} // namespace ee
