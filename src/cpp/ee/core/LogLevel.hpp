//
//  LogLevel.hpp
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#ifndef EE_X_CORE_LOG_LEVEL_HPP_
#define EE_X_CORE_LOG_LEVEL_HPP_

#ifdef __cplusplus

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
enum class LogLevel {
    Verbose,
    Debug,
    Info,
    Warn,
    Error,
    Assert,
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_CORE_LOG_LEVEL_HPP_ */
