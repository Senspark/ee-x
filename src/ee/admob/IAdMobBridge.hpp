//
//  IAdMobBridge.hpp
//  ee-x-a41c1ed3
//
//  Created by eps on 8/20/20.
//

#ifndef EE_X_I_ADMOB_BRIDGE_HPP
#define EE_X_I_ADMOB_BRIDGE_HPP

#ifdef __cplusplus

#include <memory>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/AdMobFwd.hpp"

namespace ee {
namespace admob {
class IBridge : public IPlugin {
public:
    /// Optional.
    /// Calls this method for faster ads loading.
    /// https://groups.google.com/forum/#!topic/google-admob-ads-sdk/N5okpElFHZs
    /// https://groups.google.com/forum/#!topic/google-admob-ads-sdk/kayQ3VZJqkU
    [[nodiscard]] virtual Task<bool> initialize() = 0;

    /// kGADSimulatorID or AdRequest.DEVICE_ID_EMULATOR.
    virtual std::string getEmulatorTestDeviceHash() const = 0;

    virtual void addTestDevice(const std::string& hash) = 0;

    /// Creates a banner ad.
    /// @param[in] adId The banner ad unit ID.
    /// @param[in] adSize The banner ad size.
    /// @return A reference to the banner ad if it created successfully, null
    /// otherwise.
    virtual std::shared_ptr<IAdView> createBannerAd(const std::string& adId,
                                                    BannerAdSize adSize) = 0;

    virtual std::shared_ptr<IAdView>
    createNativeAd(const std::string& adId, const std::string& layoutName,
                   const NativeAdLayout& identifiers) = 0;

    /// Creates an interstitial ad.
    /// @param[in] adId The ad unit ID.
    virtual std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId) = 0;

    /// Creates an rewarded ad.
    /// @param[in] adId The ad unit ID.
    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) = 0;
};
} // namespace admob
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_ADMOB_BRIDGE_HPP */
