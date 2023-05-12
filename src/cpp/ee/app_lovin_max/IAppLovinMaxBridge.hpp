#ifndef EE_X_I_APP_LOVIN_MAX_BRIDGE_HPP
#define EE_X_I_APP_LOVIN_MAX_BRIDGE_HPP

#ifdef __cplusplus

#include <memory>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/app_lovin_max/AppLovinMaxFwd.hpp"

namespace ee {
namespace app_lovin_max {
class IBridge : public IPlugin {
public:
    [[nodiscard]] virtual Task<bool> initialize(
        const std::string& bannerAdId,
        const std::string& rewardedAdId,
        const std::string& interstitialAdId) = 0;

    virtual std::shared_ptr<IBannerAd> createBannerAd(const std::string& adId, MaxBannerAdSize adSize) = 0;

    /// Creates an interstitial ad with the specified placement ID.
    virtual std::shared_ptr<IFullScreenAd> createInterstitialAd(const std::string& adId) = 0;

    /// Creates a rewarded video with the specified placement ID.
    virtual std::shared_ptr<IFullScreenAd> createRewardedAd(const std::string& adId) = 0;
};
} // namespace app_lovin_max
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_APP_LOVIN_MAX_BRIDGE_HPP */
