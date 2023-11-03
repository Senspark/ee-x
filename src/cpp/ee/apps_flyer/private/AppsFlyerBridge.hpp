//
//  AppsFlyerBridge.hpp
//  ee_x_mobile_apps_flyer
//
//  Created by eps on 6/26/18.
//

#ifndef EE_X_APPS_FLYER_BRIDGE_HPP
#define EE_X_APPS_FLYER_BRIDGE_HPP

#include "ee/apps_flyer/IAppsFlyerBridge.hpp"

namespace ee {
namespace apps_flyer {
class Bridge : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual void initialize(const std::string& devKey,
                            const std::string& iosAppId,
                            const std::string& appIdentity
                            ) override;
    virtual void startTracking() override;
    virtual std::string getDeviceId() const override;
    virtual void setDebugEnabled(bool enabled) override;
    virtual void setStopTracking(bool enabled) override;
    virtual void
    trackEvent(const std::string& name,
               const std::map<std::string, std::string>& values) override;

    void logRevenue(const ee::core::ILibraryAnalytics::AdRevenue &adRevenue) override;

    void logRevenue(const ee::core::ILibraryAnalytics::IapRevenue &iapRevenue) override;

    void pushGameLevel(int levelNo, const std::string &levelMode) override {}

    void popGameLevel(bool winGame) override {}

private:
    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
    bool initialized_ {false};
    std::string appIdentity_ {};
};
} // namespace apps_flyer
} // namespace ee

#endif /* EE_X_APPS_FLYER_BRIDGE_HPP */
