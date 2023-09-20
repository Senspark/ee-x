//
//  FirebaseRemoteConfig.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP
#define EE_X_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP

#include <functional>
#include "ee/firebase_remote_config/IFirebaseRemoteConfigBridge.hpp"

namespace ee {
namespace firebase {
namespace remote_config {
class Bridge : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual Task<bool> initialize() override;
    virtual Task<> setSettings(std::int64_t fetchTimeOut,
                               std::int64_t fetchInterval) override;
    virtual Task<FetchStatus> fetch(std::int64_t fetchInterval) override;
    virtual Task<bool> activate() override;
    virtual Task<> setDefaults(
        const std::unordered_map<
            std::string, std::variant<bool, std::int64_t, double, std::string>>&
            defaults) override;
    virtual bool getBool(const std::string& key) override;
    virtual std::int64_t getLong(const std::string& key) override;
    virtual double getDouble(const std::string& key) override;
    virtual std::string getString(const std::string& key) override;

private:
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
};
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_REMOTE_CONFIG_BRIDGE_HPP */
