//
//  AppLovinRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_APP_LOVIN_REWARDED_AD_HPP
#define EE_X_APP_LOVIN_REWARDED_AD_HPP

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/app_lovin/AppLovinFwd.hpp"

namespace ee {
namespace app_lovin {
class RewardedAd : public IFullScreenAd, public ObserverManager<AdObserver> {
public:
    explicit RewardedAd(
        ILogger& logger,
        const std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>>& displayer,
        Bridge* plugin);
    virtual ~RewardedAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<FullScreenAdResult> show() override;

private:
    friend Bridge;

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onClicked();
    void onClosed(bool rewarded);

    ILogger& logger_;
    std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>> displayer_;
    Bridge* plugin_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace app_lovin
} // namespace ee

#endif /* EE_X_APP_LOVIN_REWARDED_AD_HPP */
