//
//  UnityRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_UNITY_REWARDED_AD_HPP
#define EE_X_UNITY_REWARDED_AD_HPP

#include <string>

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/unity_ads/UnityAdsFwd.hpp"

namespace ee {
namespace unity_ads {
class RewardedAd : public IFullScreenAd, public ObserverManager<AdObserver> {
public:
    explicit RewardedAd(
        const Logger& logger,
        const std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>>& displayer,
        Bridge* plugin, const std::string& adId);

    virtual ~RewardedAd() override;

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<FullScreenAdResult> show() override;

private:
    friend Bridge;

    void onLoaded();
    void onFailedToShow(const std::string& message);
    void onClosed(bool rewarded);

    const Logger& logger_;
    std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>> displayer_;
    Bridge* plugin_;
    std::string adId_;
};
} // namespace unity_ads
} // namespace ee

#endif /* EE_X_UNITY_REWARDED_AD_HPP */
