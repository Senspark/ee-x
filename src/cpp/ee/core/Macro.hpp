//
//  Macro.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_MACRO_HPP
#define EE_X_MACRO_HPP

#if defined(__ANDROID__)
#define EE_X_ANDROID
#define EE_X_MOBILE
#define EE_X_COROUTINE
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#define EE_X_IOS
#define EE_X_MOBILE
#elif TARGET_OS_IPHONE
#define EE_X_IOS
#define EE_X_MOBILE
#elif TARGET_OS_MAC
#define EE_X_OSX
#define EE_X_DESKTOP
#endif // TARGET_IPHONE_SIMULATOR
#endif // __ANDROID__

#endif /* EE_X_MACRO_HPP */
