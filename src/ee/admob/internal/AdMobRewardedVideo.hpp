//
//  AdMobRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#ifndef EE_X_ADMOB_REWARDED_VIDEO_HPP
#define EE_X_ADMOB_REWARDED_VIDEO_HPP

#include "ee/CoreFwd.hpp"
#include "ee/AdMobFwd.hpp"
#include "ee/ads/IRewardedVideo.hpp"

namespace ee {
namespace admob {
class RewardedVideo : public IRewardedVideo {
private:
    using Super = IRewardedVideo;

public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit RewardedVideo(const Logger& logger, AdMob* plugin,
                           const std::string& adId);

private:
    friend AdMob;

    const Logger& logger_;
    AdMob* plugin_;
    std::string adId_;
};
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_REWARDED_VIDEO_HPP */
