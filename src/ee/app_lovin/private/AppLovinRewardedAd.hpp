//
//  AppLovinRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_APP_LOVIN_REWARDED_AD_HPP
#define EE_X_APP_LOVIN_REWARDED_AD_HPP

#include <ee/ads/IRewardedAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/AppLovinFwd.hpp"

namespace ee {
namespace app_lovin {
class RewardedAd : public IRewardedAd,
                   public ObserverManager<IRewardedAdObserver> {
public:
    virtual ~RewardedAd() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<IRewardedAdResult> show() override;

private:
    friend Bridge;

    explicit RewardedAd(Bridge* plugin);

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onClicked();
    void onClosed(bool rewarded);

    Bridge* plugin_;

    std::unique_ptr<ads::AsyncHelper<bool>> loader_;
    std::unique_ptr<ads::AsyncHelper<IRewardedAdResult>> displayer_;
};
} // namespace app_lovin
} // namespace ee

#endif /* EE_X_APP_LOVIN_REWARDED_AD_HPP */
