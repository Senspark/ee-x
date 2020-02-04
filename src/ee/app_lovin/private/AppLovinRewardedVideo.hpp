//
//  AppLovinRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_APP_LOVIN_REWARDED_VIDEO_HPP
#define EE_X_APP_LOVIN_REWARDED_VIDEO_HPP

#include <ee/ads/IRewardedVideo.hpp>

#include "ee/AppLovinFwd.hpp"

namespace ee {
namespace app_lovin {
class RewardedVideo : public IRewardedVideo {
private:
    using Super = IRewardedVideo;

public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit RewardedVideo(const Logger& logger, Bridge* plugin);

private:
    friend Bridge;

    Bridge* plugin_;
};
} // namespace app_lovin
} // namespace ee

#endif /* EE_X_APP_LOVIN_REWARDED_VIDEO_HPP */
