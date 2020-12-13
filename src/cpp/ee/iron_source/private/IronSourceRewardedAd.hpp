//
//  IronSourceRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_IRON_SOURCE_REWARDED_AD_HPP
#define EE_X_IRON_SOURCE_REWARDED_AD_HPP

#include <string>

#include <ee/ads/IRewardedAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/iron_source/IronSourceFwd.hpp"

namespace ee {
namespace iron_source {
class RewardedAd : public IRewardedAd,
                   public ObserverManager<IRewardedAdObserver> {
public:
    virtual ~RewardedAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<IRewardedAdResult> show() override;

private:
    friend Bridge;

    explicit RewardedAd(
        const Logger& logger,
        std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>>& displayer,
        Bridge* plugin, const std::string& adId);

    void onLoaded();
    void onFailedToShow(const std::string& message);
    void onClicked();
    void onClosed(bool rewarded);

    const Logger& logger_;
    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> displayer_;
    Bridge* plugin_;
    std::string adId_;
};
} // namespace iron_source
} // namespace ee

#endif /* EE_X_IRON_SOURCE_REWARDED_AD_HPP */
