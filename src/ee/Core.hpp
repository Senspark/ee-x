//
//  Core.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_CORE_HEADER_HPP_
#define EE_X_CORE_HEADER_HPP_

#include "ee/Macro.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Metrics.hpp"
#include "ee/core/Utils.hpp"

#if defined(EE_X_ANDROID)
#include "ee/core/JniUtils.hpp"
#elif defined(EE_X_IOS) || defined(EE_X_OSX)
#if defined(__OBJC__)
#import "ee/core/EEMetrics.h"
#import "ee/core/EEPluginManager.h"
#endif // __OBJC__
#endif // EE_X_ANDROID

namespace ee {
using core::Logger;
using core::LogLevel;

#if defined(EE_X_ANDROID)
using core::JniUtils;
#endif // EE_X_ANDROID
} // namespace ee

#endif /* EE_X_CORE_HEADER_HPP_ */
