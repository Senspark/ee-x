//
//  ALovinAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_APP_LOVIN_BRIDGE_HPP
#define EE_X_APP_LOVIN_BRIDGE_HPP

#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace applovin {
class AppLovin final : public RewardedVideoInterface {
public:
    static const std::string DefaultPlacementId;

    AppLovin();
    virtual ~AppLovin() override;

    void initialize();

    bool isInterstitialAdReady() const;

    bool showInterstitialAd();

    /// @see Super.
    virtual bool
    isRewardedVideoReady(const std::string& placementId) const override;

    /// @see Super.
    virtual bool showRewardedVideo(const std::string& placementId) override;
};
} // namespace applovin
} // namespace ee

#endif /* EE_X_APP_LOVIN_BRIDGE_HPP */
