//
//  FirebaseAnalytics.cpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/firebase/FirebaseAnalytics.hpp"
#include "ee/firebase/FirebaseApp.hpp"

#ifndef __MACH__
#include <firebase/analytics.h>
#endif // __MACH__

namespace ee {
namespace firebase {
FirebaseAnalytics::FirebaseAnalytics() {
    initialized_ = false;
}

FirebaseAnalytics::~FirebaseAnalytics() {
    if (initialized_) {
#ifndef __MACH__
        ::firebase::analytics::Terminate();
#endif // __MACH__
    }
}

bool FirebaseAnalytics::initialize() {
    if (initialized_) {
        return true;
    }

    FirebaseApp::initialize();

#ifndef __MACH__
    auto app = ::firebase::App::GetInstance();
    if (app == nullptr) {
        return false;
    }

    ::firebase::analytics::Initialize(*app);
    ::firebase::analytics::SetAnalyticsCollectionEnabled(true);
    ::firebase::analytics::SetMinimumSessionDuration(10000);
    ::firebase::analytics::SetSessionTimeoutDuration(1800000);
#endif // __MACH__

    initialized_ = true;
    return true;
}

void FirebaseAnalytics::setUserProperty(const std::string& name,
                                        const std::string& property) {
    if (not initialized_) {
        return;
    }
#ifndef __MACH__
    ::firebase::analytics::SetUserProperty(name.c_str(), property.c_str());
#endif // __MACH__
}
} // namespace firebase
} // namespace ee
