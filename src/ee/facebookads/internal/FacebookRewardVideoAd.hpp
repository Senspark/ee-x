//
//  FacebookRewardVideoAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_FACEBOOK_REWARDED_VIDEO_HPP
#define EE_X_FACEBOOK_REWARDED_VIDEO_HPP

#include "ee/FacebookAdsFwd.hpp"
#include "ee/ads/IInterstitialAd.hpp"

namespace ee {
namespace facebook {
class RewardedVideo : public IRewardedVideo {
private:
    using Super = IRewardedVideo;

public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    friend FacebookAds;

    explicit RewardedVideo(IMessageBridge& bridge, const Logger& logger,
                           const std::string& adId);

private:
    std::string adId_;
    bool rewarded_{false};
    bool loading_{false};

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onReward();
    void onOpened();
    void onClosed();

    void createInternalVideo();
    void destroyInternalVideo();

    IMessageBridge& bridge_;
    const Logger& logger_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_REWARDED_VIDEO_HPP */
