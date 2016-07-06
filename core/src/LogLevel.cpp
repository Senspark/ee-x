//
//  LogLevel.cpp
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#include "LogLevel.hpp"

namespace ee {
namespace core {
LogLevel::LogLevel(int priority_, const std::string& desc_)
    : priority{priority_}
    , desc{desc_} {}

const LogLevel LogLevel::Verbose(2, "[VERBOSE]");
const LogLevel LogLevel::Debug(3, "[DEBUG]");
const LogLevel LogLevel::Info(4, "[INFO]");
const LogLevel LogLevel::Warn(5, "[WARN]");
const LogLevel LogLevel::Error(6, "[ERROR]");
const LogLevel LogLevel::Assert(7, "[ASSERT]");
} // namespace core
} // namespace ee