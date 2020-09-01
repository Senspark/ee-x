//
//  AdjustFwd.hpp
//  Adjust
//
//  Created by eps on 8/19/20.
//

#ifndef EE_X_ADJUST_FWD_HPP
#define EE_X_ADJUST_FWD_HPP

#ifdef __cplusplus

#include <ee/CoreFwd.hpp>

namespace ee {
namespace adjust {
enum class Environment;
enum class LogLevel;

class IBridge;
class Bridge;
class Config;
} // namespace adjust

using IAdjust = adjust::IBridge;
using AdjustConfig = adjust::Config;
using AdjustEnvironment = adjust::Environment;
using AdjustLogLevel = adjust::LogLevel;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADJUST_FWD_HPP */
