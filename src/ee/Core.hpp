//
//  Core.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_CORE_HEADER_HPP_
#define EE_X_CORE_HEADER_HPP_

#ifdef __cplusplus
#include "ee/CoreFwd.hpp"
#include "ee/core/IService.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Metrics.hpp"
#include "ee/core/ObserverManager.hpp"
#include "ee/core/PluginManager.hpp"
#include "ee/core/ServiceLocator.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/VideoPlayer.hpp"
#include "ee/core/VideoPlayerManager.hpp"
#endif // __cplusplus

#if defined(EE_X_ANDROID)
#ifdef __cplusplus
#include "ee/core/JniUtils.hpp"
#endif // __cplusplus
#elif defined(EE_X_IOS) || defined(EE_X_OSX)
#if defined(__OBJC__)
#import "ee/core/EEPluginManager.h"
#endif // __OBJC__
#endif // EE_X_ANDROID

#endif /* EE_X_CORE_HEADER_HPP_ */
