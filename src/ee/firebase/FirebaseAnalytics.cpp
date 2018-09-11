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
#include <firebase/analytics/parameter_names.h>
#endif // EE_X_MOBILE

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
            setMinimumSessionDuration(10000);
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

        void Self::setMinimumSessionDuration(std::int64_t milliseconds) {
            if (not initialized_) {
                return;
            }
#ifdef EE_X_MOBILE
            ::firebase::analytics::SetMinimumSessionDuration(milliseconds);
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

        void Self::logEvent(const std::string& name) {
#ifdef EE_X_MOBILE
            ::firebase::analytics::LogEvent(name.c_str());
#endif // EE_X_MOBILE
        }

        void Self::logScreen(const std::string& screenName) const {
#ifdef EE_X_MOBILE
            ::firebase::analytics::LogEvent("screen",
                                            ::firebase::analytics::kParameterItemName,
                                            screenName.c_str());
#endif // EE_X_MOBILE
        }

        void Self::logCustomEvent(const std::string& category,
                                  const std::string& action, const std::string& label,
                                  float value) const {
#ifdef EE_X_MOBILE
            ::firebase::analytics::Parameter parameters[] = {
                    ::firebase::analytics::Parameter("category", category),
                    ::firebase::analytics::Parameter("action", action),
                    ::firebase::analytics::Parameter("label", label),
                    ::firebase::analytics::Parameter("value", value),
            };
            ::firebase::analytics::LogEvent("event", parameters,
                                            sizeof(parameters) / sizeof(parameters[0]));
#endif // EE_X_MOBILE
        }

        void Self::logEcommerce(const std::string& productId,
                                const std::string& productName,
                                const std::string& category, float price,
                                bool success) const {
#ifdef EE_X_MOBILE
            ::firebase::analytics::Parameter parameters[] = {
                    ::firebase::analytics::Parameter(
                            ::firebase::analytics::kParameterItemID, productId),
                    ::firebase::analytics::Parameter(
                            ::firebase::analytics::kParameterItemName, productName),
                    ::firebase::analytics::Parameter(
                            ::firebase::analytics::kParameterItemCategory, category),
                    ::firebase::analytics::Parameter(::firebase::analytics::kParameterPrice,
                                                     price),
                    ::firebase::analytics::Parameter(
                            ::firebase::analytics::kParameterSuccess, success),
            };
            ::firebase::analytics::LogEvent("buy", parameters,
                                            sizeof(parameters) / sizeof(parameters[0]));
#endif // EE_X_MOBILE
        }

        void Self::logGameStart(const std::string& type,
                                const std::string& difficulty) const {
            ::firebase::analytics::Parameter parameters[] = {
                    ::firebase::analytics::Parameter("type", type),
                    ::firebase::analytics::Parameter("difficulty", difficulty),
            };
            ::firebase::analytics::LogEvent("game_start", parameters,
                                            sizeof(parameters) / sizeof(parameters[0]));
        }

        void Self::logGameEnd(const std::string& type, const std::string& difficulty,
                              int rank) const {
            ::firebase::analytics::Parameter parameters[] = {
                    ::firebase::analytics::Parameter("type", type),
                    ::firebase::analytics::Parameter("difficulty", difficulty),
                    ::firebase::analytics::Parameter("rank", rank),
            };
            ::firebase::analytics::LogEvent("game_end", parameters,
                                            sizeof(parameters) / sizeof(parameters[0]));
        }
    } // namespace firebase
} // namespace ee
