//
//  UnityRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_UNITY_REWARDED_VIDEO_HPP
#define EE_X_UNITY_REWARDED_VIDEO_HPP

#include "ee/UnityAdsFwd.hpp"
#include "ee/ads/IRewardedVideo.hpp"

namespace ee {
namespace unityads {
class RewardedVideo : public IRewardedVideo {
public:
    virtual ~RewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit RewardedVideo(UnityAds* plugin, const std::string& placementId);

private:
    friend UnityAds;

    UnityAds* plugin_;
    std::string placementId_;
};
} // namespace unityads
} // namespace ee

#endif /* EE_X_UNITY_REWARDED_VIDEO_HPP */
