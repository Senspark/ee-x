//
//  FirebaseAnalytics.cpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/firebase/analytics/FirebaseAnalytics.hpp"

#if defined(EE_X_MOBILE)
#include <firebase/analytics.h>
#include <firebase/analytics/parameter_names.h>
#endif // EE_X_MOBILE

#include <ee/firebase/core/FirebaseApp.hpp>

namespace ee {
namespace firebase {
using Self = Analytics;

Self::Analytics() {
    initialized_ = false;
}

Self::~Analytics() {
    if (initialized_) {
#if defined(EE_X_MOBILE)
        ::firebase::analytics::Terminate();
#endif // EE_X_MOBILE
    }
}

bool Self::initialize() {
    if (initialized_) {
        return true;
    }

    App::initialize();

#if defined(EE_X_MOBILE)
    auto app = ::firebase::App::GetInstance();
    if (app == nullptr) {
        return false;
    }

    ::firebase::analytics::Initialize(*app);
    analyticsCollectionEnabled(true);
    setSessionTimeoutDuration(1800000);
#endif // defined(EE_X_MOBILE)

    initialized_ = true;
    return true;
}

void Self::analyticsCollectionEnabled(bool enabled) {
    if (not initialized_) {
        return;
    }
#ifdef EE_X_MOBILE
    ::firebase::analytics::SetAnalyticsCollectionEnabled(enabled);
#endif // EE_X_MOBILE
}

void Self::setSessionTimeoutDuration(std::int64_t milliseconds) {
    if (not initialized_) {
        return;
    }
#ifdef EE_X_MOBILE
    ::firebase::analytics::SetSessionTimeoutDuration(milliseconds);
#endif // EE_X_MOBILE
}

void Self::setUserId(const std::string& userId) {
    if (not initialized_) {
        return;
    }
#ifdef EE_X_MOBILE
    ::firebase::analytics::SetUserId(userId.c_str());
#endif // EE_X_MOBILE
}

void Self::setUserProperty(const std::string& name,
                           const std::string& property) {
    if (not initialized_) {
        return;
    }
#if defined(EE_X_MOBILE)
    ::firebase::analytics::SetUserProperty(name.c_str(), property.c_str());
#endif // EE_X_MOBILE
}

void Self::logEvent(const std::string& name, const TrackingDict& dict) {
    if (not initialized_) {
        return;
    }
#ifdef EE_X_MOBILE
    if (dict.empty()) {
        ::firebase::analytics::LogEvent(name.c_str());
    } else {
        auto parameters =
            std::make_unique<::firebase::analytics::Parameter[]>(dict.size());
        std::size_t index = 0;
        for (const auto& item : dict) {
            parameters[index++] = ::firebase::analytics::Parameter(
                item.first.c_str(), item.second.c_str());
        }

        ::firebase::analytics::LogEvent(name.c_str(), parameters.get(),
                                        dict.size());
    }

#endif // EE_X_MOBILE
}
} // namespace firebase
} // namespace ee
