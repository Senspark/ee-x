//
//  VungleRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_VUNGLE_REWARDED_VIDEO_HPP
#define EE_X_VUNGLE_REWARDED_VIDEO_HPP

#include <string>

#include <ee/ads/IRewardedVideo.hpp>

#include "ee/VungleFwd.hpp"

namespace ee {
namespace vungle {
class RewardedVideo : public IRewardedVideo {
private:
    using Super = IRewardedVideo;

public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit RewardedVideo(const Logger& logger, Vungle* plugin,
                           const std::string& placementId);

private:
    friend Vungle;

    const Logger& logger_;
    Vungle* plugin_;
    std::string placementId_;
};
} // namespace vungle
} // namespace ee

#endif /* EE_X_VUNGLE_REWARDED_VIDEO_HPP */
