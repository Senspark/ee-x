//
//  UnityRewardedVideo.hpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#ifndef EE_X_UNITY_REWARDED_VIDEO_HPP
#define EE_X_UNITY_REWARDED_VIDEO_HPP

#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace unityads {
class UnityAds;

class UnityRewardedVideo : public RewardedVideoInterface {
public:
    virtual ~UnityRewardedVideo() override;

    virtual bool isLoaded() const override;

    virtual void load() override;

    virtual bool show() override;

protected:
    explicit UnityRewardedVideo(UnityAds* plugin,
                                const std::string& placementId);

private:
    friend UnityAds;

    UnityAds* plugin_;
    std::string placementId_;
};
} // namespace unityads
} // namespace ee

#endif /* EE_X_UNITY_REWARDED_VIDEO_HPP */
