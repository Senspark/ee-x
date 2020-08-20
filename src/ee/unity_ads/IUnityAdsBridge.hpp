//
//  IUnityAdsBridge.hpp
//  ee-x-a41c1ed3
//
//  Created by eps on 8/20/20.
//

#ifndef EE_X_I_UNITY_ADS_BRIDGE_HPP
#define EE_X_I_UNITY_ADS_BRIDGE_HPP

#ifdef __cplusplus

#include <memory>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/UnityAdsFwd.hpp"

namespace ee {
namespace unity_ads {
class IBridge : public IPlugin {
public:
    /// Initializes Unity Ads.
    /// @param[in] gameId Unity Ads game ID.
    /// @param[in] testModeEnabled Whether test mode is enabled.
    virtual void initialize(const std::string& gameId,
                            bool testModeEnabled) = 0;

    /// Sets debug mode enabled.
    virtual void setDebugModeEnabled(bool enabled) = 0;

    /// Creates an interstitial ad with the specified placement ID.
    virtual std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId) = 0;

    /// Creates a rewarded video with the specified placement ID.
    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) = 0;
};
} // namespace unity_ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_UNITY_ADS_BRIDGE_HPP */
