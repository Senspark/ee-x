//
//  UnityAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_UNITY_ADS_BRIDGE_HPP
#define EE_X_UNITY_ADS_BRIDGE_HPP

#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace unityads {
class UnityAds final : public RewardedVideoInterface {
public:
    UnityAds();
    virtual ~UnityAds() override;

    void initialize(const std::string& gameId);

    /// @see Super.
    virtual bool showRewardedVideo(const std::string& placementId) override;
};
} // namespace unityads
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
