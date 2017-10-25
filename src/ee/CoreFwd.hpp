//
//  CoreFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_CORE_FWD_HPP
#define EE_X_CORE_FWD_HPP

#include "ee/Macro.hpp"

namespace ee {
namespace core {
class Logger;
struct LogLevel;

#if defined(EE_X_ANDROID)
class JniUtils;
#endif // EE_X_ANDROID
} // namespace core

using core::Logger;
using core::LogLevel;

#if defined(EE_X_ANDROID)
using core::JniUtils;
#endif // EE_X_ANDROID
} // namespace ee

#endif /* EE_X_CORE_FWD_HPP */
