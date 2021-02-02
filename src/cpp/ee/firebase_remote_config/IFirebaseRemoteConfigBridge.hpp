//
//  IFirebaseRemoteConfigBridge.hpp
//  Pods
//
//  Created by eps on 2/2/21.
//

#ifndef EE_X_I_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP
#define EE_X_I_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP

#ifdef __cplusplus

#include <unordered_map>
#include <variant>

#include <ee/core/IPlugin.hpp>

#include "ee/firebase_remote_config/FirebaseRemoteConfigFwd.hpp"

namespace ee {
namespace firebase {
namespace remote_config {
class IBridge : public IPlugin {
public:
    [[nodiscard]] virtual Task<> initialize() = 0;
    [[nodiscard]] virtual Task<> setSettings(std::int64_t fetchTimeOut,
                                             std::int64_t fetchInterval) = 0;
    [[nodiscard]] virtual Task<FetchStatus> fetch(std::int64_t fetchTimeOut) = 0;
    [[nodiscard]] virtual Task<bool> activate() = 0;
    [[nodiscard]] virtual Task<> setDefaults(
        const std::unordered_map<
            std::string, std::variant<bool, std::int64_t, double, std::string>>&
            defaults) = 0;
    virtual Task<bool> getBool(const std::string& key) = 0;
    virtual Task<std::int64_t> getLong(const std::string& key) = 0;
    virtual Task<double> getDouble(const std::string& key) = 0;
    virtual Task<std::string> getString(const std::string& key) = 0;
};
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP */
