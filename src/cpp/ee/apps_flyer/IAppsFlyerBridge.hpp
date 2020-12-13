//
//  IAppsFlyerBridge.hpp
//  ee_x
//
//  Created by eps on 6/26/18.
//

#ifndef EE_X_I_APPS_FLYER_BRIDGE_HPP
#define EE_X_I_APPS_FLYER_BRIDGE_HPP

#ifdef __cplusplus

#include <map>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/apps_flyer/AppsFlyerFwd.hpp"

namespace ee {
namespace apps_flyer {
class IBridge : public IPlugin {
public:
    virtual ~IBridge() = default;

    /// Initializes the AppsFlyer SDK.
    /// @param[in] devKey Your AppsFlyer's dev key.
    /// @param[in] appId Your app's Apple ID (taken from the app's page on
    /// iTunes Connect), without the "id" prefix.
    virtual void initialize(const std::string& devKey,
                            const std::string& appId) = 0;

    virtual void startTracking() = 0;

    /// AppsFlyer's unique device ID is created for every new install of an app.
    virtual std::string getDeviceId() const = 0;

    virtual void setDebugEnabled(bool enabled) = 0;

    virtual void setStopTracking(bool enabled) = 0;

    virtual void
    trackEvent(const std::string& name,
               const std::map<std::string, std::string>& values) = 0;
};
} // namespace apps_flyer
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_APPS_FLYER_BRIDGE_HPP */
