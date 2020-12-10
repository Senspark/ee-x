//
//  AdjustLogLevel.hpp
//  Adjust
//
//  Created by eps on 8/19/20.
//

#ifndef EE_X_ADJUST_LOG_LEVEL_HPP
#define EE_X_ADJUST_LOG_LEVEL_HPP

#ifdef __cplusplus

#include "ee/AdjustFwd.hpp"

namespace ee {
namespace adjust {
enum class LogLevel {
    /// Enables all logging.
    Verbose,

    /// Enables more logging.
    Debug,

    /// Default setting.
    Info,

    /// Disables info logging.
    Warn,

    /// Disables warnings as well.
    Error,

    /// Disables errors as well.
    Assert,

    /// Disables all log ouput.
    Suppress,
};
} // namespace adjust
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADJUST_LOG_LEVEL_HPP */
