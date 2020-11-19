//
//  IFacebookAdsBridge.hpp
//  ee-x-a41c1ed3
//
//  Created by eps on 8/20/20.
//

#ifndef EE_X_I_FACEBOOK_ADS_BRIDGE_HPP
#define EE_X_I_FACEBOOK_ADS_BRIDGE_HPP

#ifdef __cplusplus

#include <memory>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/FacebookAdsFwd.hpp"

namespace ee {
namespace facebook_ads {
class IBridge : public IPlugin {
public:
    [[nodiscard]] virtual Task<bool> initialize() = 0;

    /// Gets the current device's hash, iOS only, for Android search for
    /// AdSettings tag.
    virtual std::string getTestDeviceHash() const = 0;

    /// Adds a test device.
    /// @param[in] hash The device's hash.
    virtual void addTestDevice(const std::string& hash) = 0;

    /// Clears all test devices.
    virtual void clearTestDevices() = 0;

    /// Creates a banner ad.
    /// @param[in] adId The banner ad ID.
    /// @param[in] adSize The banner ad predefined size.
    virtual std::shared_ptr<IAdView> createBannerAd(const std::string& adId,
                                                    BannerAdSize adSize) = 0;

    /// Creates a native ad.
    /// @param[in] adId The native ad ID.
    /// @param[in] layoutName The layout name (.xml for Android, .xib for iOS).
    /// @param[in] identifiers Android only.
    virtual std::shared_ptr<IAdView>
    createNativeAd(const std::string& adId, const std::string& layoutName,
                   const NativeAdLayout& identifiers) = 0;

    /// Creates an interstitial ad.
    /// @param[in] adId The ad placement ID>
    virtual std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId) = 0;

    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) = 0;
};
} // namespace facebook_ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_FACEBOOK_ADS_BRIDGE_HPP */
