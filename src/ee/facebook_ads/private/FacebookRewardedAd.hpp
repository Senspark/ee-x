//
//  FacebookRewardVideoAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_FACEBOOK_REWARDED_AD_HPP
#define EE_X_FACEBOOK_REWARDED_AD_HPP

#include <string>

#include <ee/ads/IRewardedAd.hpp>
#include <ee/ads/internal/MessageHelper.hpp>
#include <ee/core/SafeObserverManager.hpp>

#include "ee/FacebookAdsFwd.hpp"

namespace ee {
namespace facebook_ads {
class RewardedAd : public IRewardedAd,
                   public SafeObserverManager<IRewardedAdObserver> {
public:
    virtual ~RewardedAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<IRewardedAdResult> show() override;

private:
    friend Bridge;

    explicit RewardedAd(
        IMessageBridge& bridge, const Logger& logger,
        const std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>>& displayer,
        Bridge* plugin, const std::string& adId);

    bool createInternalAd();
    bool destroyInternalAd();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onFailedToShow(const std::string& message);
    void onClicked();
    void onClosed(bool rewarded);

    IMessageBridge& bridge_;
    const Logger& logger_;
    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> displayer_;
    Bridge* plugin_;
    std::string adId_;
    ads::MessageHelper messageHelper_;

    std::unique_ptr<ads::IAsyncHelper<bool>> loader_;
};
} // namespace facebook_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_REWARDED_AD_HPP */
