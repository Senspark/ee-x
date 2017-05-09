//
//  Core.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_CORE_HEADER_HPP_
#define EE_X_CORE_HEADER_HPP_

#include "ee/core/Logger.hpp"
#include "ee/core/LogLevel.hpp"

#if defined(__ANDROID__)
#include "ee/core/JniUtils.hpp"
#elif defined(__APPLE__)
#ifdef __OBJC__
#import "ee/core/EEPluginManager.h"
#endif // __OBJC__
#endif // __ANDROID__

namespace ee {
using core::Logger;
using core::LogLevel;

#if defined(__ANDROID__)
using core::JniUtils;
#endif // __ANDROID__
} // namespace ee

#endif /* EE_X_CORE_HEADER_HPP_ */
