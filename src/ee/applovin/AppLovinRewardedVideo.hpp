//
//  AppLovinRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_APP_LOVIN_REWARDED_VIDEO_HPP
#define EE_X_APP_LOVIN_REWARDED_VIDEO_HPP

#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace applovin {
class AppLovin;

class RewardedVideo : public RewardedVideoInterface {
public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit RewardedVideo(AppLovin* plugin);

private:
    friend AppLovin;

    AppLovin* plugin_;
};
} // namespace applovin
} // namespace ee

#endif /* EE_X_APP_LOVIN_REWARDED_VIDEO_HPP */
