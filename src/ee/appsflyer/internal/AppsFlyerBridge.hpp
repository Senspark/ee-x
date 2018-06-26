//
//  AppsFlyerBridge.hpp
//  ee_x_mobile_apps_flyer
//
//  Created by eps on 6/26/18.
//

#ifndef EE_X_APPS_FLYER_BRIDGE_HPP
#define EE_X_APPS_FLYER_BRIDGE_HPP

#include "ee/CoreFwd.hpp"
#include "ee/appsflyer/IAppsFlyerBridge.hpp"

namespace ee {
namespace appsflyer {
class Bridge : public IBridge {
public:
    Bridge();
    virtual ~Bridge() override;

    virtual void initialize(const std::string& devKey,
                            const std::string& appId) override;

    virtual std::string getDeviceId() const override;

    virtual void setDebugEnabled(bool enabled) override;

    virtual void
    trackEvent(const std::string& name,
               const std::map<std::string, std::string>& values) override;

private:
    IMessageBridge& bridge_;
};
} // namespace appsflyer
} // namespace ee

#endif /* EE_X_APPS_FLYER_BRIDGE_HPP */
