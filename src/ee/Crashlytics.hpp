//
//  Crashlytics.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_CRASHLYTICS_HEADER_HPP_
#define EE_X_CRASHLYTICS_HEADER_HPP_

#include <ee/Core.hpp>

#include "ee/CrashlyticsFwd.hpp"
#include "ee/crashlytics/CrashlyticsBridge.hpp"

#ifdef __OBJC__
#import "ee/crashlytics/EECrashlytics.h"
#endif // __OBJC__

#endif /* EE_X_CRASHLYTICS_HEADER_HPP_ */
