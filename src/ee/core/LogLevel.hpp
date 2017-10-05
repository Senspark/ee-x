//
//  LogLevel.hpp
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#ifndef EE_X_CORE_LOG_LEVEL_HPP_
#define EE_X_CORE_LOG_LEVEL_HPP_

#include <string>

namespace ee {
namespace core {
struct LogLevel {
    static const LogLevel Verbose;
    static const LogLevel Debug;
    static const LogLevel Info;
    static const LogLevel Warn;
    static const LogLevel Error;
    static const LogLevel Assert;

    // Log level priorities are specified here:
    // https://developer.android.com/reference/android/util/Log.html
    explicit LogLevel(int priority, const std::string& desc);

    int priority;
    std::string desc;
};
} // namespace core

using core::LogLevel;
} // namespace ee

#endif /* EE_X_CORE_LOG_LEVEL_HPP_ */
