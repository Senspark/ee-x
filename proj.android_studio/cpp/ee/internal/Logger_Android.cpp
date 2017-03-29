//
//  Logger-Android.cpp
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#include <android/log.h>

#include "ee/Logger.hpp"
#include "ee/LogLevel.hpp"

namespace ee {
namespace core {
void Logger::log0(const LogLevel& level, const std::string& tag,
                  const std::string& message) {
    __android_log_print(level.priority, tag.c_str(), "%s", message.c_str());
}
} // namespace core
} // namespace ee