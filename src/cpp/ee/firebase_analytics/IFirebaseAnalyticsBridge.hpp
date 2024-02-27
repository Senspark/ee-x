#ifndef EE_X_I_FIREBASE_ANALYTICS_BRIDGE_HPP
#define EE_X_I_FIREBASE_ANALYTICS_BRIDGE_HPP

#ifdef __cplusplus

#include <unordered_map>
#include <variant>

#include <ee/core/IPlugin.hpp>
#include <ee/core/ILibraryAnalytics.h>

#include "ee/firebase_analytics/FirebaseAnalyticsFwd.hpp"

namespace ee::firebase::analytics {
    class IBridge : public IPlugin,
                    public ILibraryAnalytics {
    public:
        [[nodiscard]] virtual Task<bool> initialize() = 0;

        virtual void setUserProperty(const std::string &key,
                                     const std::string &value) = 0;

        virtual void setAppIdentifier(const std::string& appIdentifier) = 0;

        virtual void trackScreen(const std::string &name) = 0;

        virtual void logEvent(const std::string &name,
                              const std::unordered_map<std::string, std::variant<std::int64_t, double, std::string>> &parameters) = 0;
    };
} // namespace

#endif // __cplusplus

#endif // EE_X_I_FIREBASE_ANALYTICS_BRIDGE_HPP
