//
//  FirebaseAnalytics.cpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/firebase/FirebaseAnalytics.hpp"
#include "ee/firebase/FirebaseApp.hpp"

#if defined(EE_X_MOBILE)
#include <firebase/analytics.h>
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
FirebaseAnalytics::FirebaseAnalytics() {
    initialized_ = false;
}

FirebaseAnalytics::~FirebaseAnalytics() {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::analytics::Terminate();
#endif // EE_X_MOBILE
    }
}

bool FirebaseAnalytics::initialize() {
    if (initialized_) {
        return true;
    }

    FirebaseApp::initialize();

#if defined(EE_X_MOBILE)
    auto app = ::firebase::App::GetInstance();
    if (app == nullptr) {
        return false;
    }

    ::firebase::analytics::Initialize(*app);
    ::firebase::analytics::SetAnalyticsCollectionEnabled(true);
    ::firebase::analytics::SetMinimumSessionDuration(10000);
    ::firebase::analytics::SetSessionTimeoutDuration(1800000);
#endif // defined(EE_X_MOBILE)

    initialized_ = true;
    return true;
}

void FirebaseAnalytics::setUserProperty(const std::string& name,
                                        const std::string& property) {
    if (not initialized_) {
        return;
    }
#if defined(EE_X_MOBILE)
    ::firebase::analytics::SetUserProperty(name.c_str(), property.c_str());
#endif // EE_X_MOBILE
}
} // namespace firebase
} // namespace ee
