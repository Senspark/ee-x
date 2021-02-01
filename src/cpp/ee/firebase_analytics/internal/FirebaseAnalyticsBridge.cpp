//
//  FirebaseAnalytics.cpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/firebase_analytics/FirebaseAnalyticsBridge.hpp"

#include <firebase/analytics.h>
#include <firebase/analytics/parameter_names.h>

#include <ee/firebase_core/FirebaseCoreBridge.hpp>

namespace ee {
namespace firebase {
namespace analytics {
using Self = Bridge;

Self::Bridge() {
    initialized_ = false;
}

Self::~Bridge() {
    if (initialized_) {
        ::firebase::analytics::Terminate();
    }
}

bool Self::initialize() {
    if (initialized_) {
        return true;
    }

    FirebaseCore::initialize();

    auto app = ::firebase::App::GetInstance();
    if (app == nullptr) {
        return false;
    }

    ::firebase::analytics::Initialize(*app);
    analyticsCollectionEnabled(true);
    setSessionTimeoutDuration(1800000);

    initialized_ = true;
    return true;
}

void Self::analyticsCollectionEnabled(bool enabled) {
    if (not initialized_) {
        return;
    }
    ::firebase::analytics::SetAnalyticsCollectionEnabled(enabled);
}

void Self::setSessionTimeoutDuration(std::int64_t milliseconds) {
    if (not initialized_) {
        return;
    }
    ::firebase::analytics::SetSessionTimeoutDuration(milliseconds);
}

void Self::setUserId(const std::string& userId) {
    if (not initialized_) {
        return;
    }
    ::firebase::analytics::SetUserId(userId.c_str());
}

void Self::setUserProperty(const std::string& name,
                           const std::string& property) {
    if (not initialized_) {
        return;
    }
    ::firebase::analytics::SetUserProperty(name.c_str(), property.c_str());
}

void Self::setCurrentScreen(const std::string& screenName,
                            const std::optional<std::string>& screenClass) {
    if (not initialized_) {
        return;
    }
    ::firebase::analytics::SetCurrentScreen(
        screenName.c_str(),
        screenClass.has_value() ? screenClass->c_str() : nullptr);
}

void Self::logEvent(const std::string& name, const TrackingDict& dict) {
    if (not initialized_) {
        return;
    }
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
}
} // namespace analytics
} // namespace firebase
} // namespace ee
