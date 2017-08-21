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
    analyticsCollectionEnabled(true);
    setMinimumSessionDuration(10000);
    setSessionTimeoutDuration(1800000);
#endif // defined(EE_X_MOBILE)

    initialized_ = true;
    return true;
}

void FirebaseAnalytics::analyticsCollectionEnabled(bool enabled) {
    if (not initialized_) {
        return;
    }
#ifdef EE_X_MOBILE
    ::firebase::analytics::SetAnalyticsCollectionEnabled(enabled);
#endif // EE_X_MOBILE
}

void FirebaseAnalytics::setMinimumSessionDuration(std::int64_t milliseconds) {
    if (not initialized_) {
        return;
    }
#ifdef EE_X_MOBILE
    ::firebase::analytics::SetMinimumSessionDuration(milliseconds);
#endif // EE_X_MOBILE
}

void FirebaseAnalytics::setSessionTimeoutDuration(std::int64_t milliseconds) {
    if (not initialized_) {
        return;
    }
#ifdef EE_X_MOBILE
    ::firebase::analytics::SetSessionTimeoutDuration(milliseconds);
#endif // EE_X_MOBILE
}

void FirebaseAnalytics::setUserId(const std::string& userId) {
    if (not initialized_) {
        return;
    }
#ifdef EE_X_MOBILE
    ::firebase::analytics::SetUserId(userId.c_str());
#endif // EE_X_MOBILE
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

void FirebaseAnalytics::logEvent(const std::string& name) {
#ifdef EE_X_MOBILE
    ::firebase::analytics::LogEvent(name.c_str());
#endif // EE_X_MOBILE
}
} // namespace firebase
} // namespace ee
