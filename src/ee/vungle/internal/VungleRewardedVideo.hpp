//
//  VungleRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_VUNGLE_REWARDED_VIDEO_HPP
#define EE_X_VUNGLE_REWARDED_VIDEO_HPP

#include "ee/VungleFwd.hpp"
#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace vungle {
class RewardedVideo : public RewardedVideoInterface {
public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit RewardedVideo(Vungle* plugin, const std::string& placementId);

private:
    friend Vungle;

    Vungle* plugin_;
    std::string placementId_;
};
} // namespace vungle
} // namespace ee

#endif /* EE_X_VUNGLE_REWARDED_VIDEO_HPP */
