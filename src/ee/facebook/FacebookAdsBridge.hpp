//
//  FacebookAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_FACEBOOK_ADS_BRIDGE_HPP_
#define EE_X_FACEBOOK_ADS_BRIDGE_HPP_

#include <functional>
#include <memory>
#include <string>

#include "ee/ads/AdViewInterface.hpp"
#include "ee/ads/InterstitialAdInterface.hpp"

namespace ee {
namespace facebook {
class BannerAd;
class NativeAd;
class NativeAdBuilder;
class InterstitialAd;

enum class FacebookBannerAdSize {
    /// kFBAdSizeHeight50Banner or AdSize.BANNER_HEIGHT_50.
    BannerHeight50,

    /// kFBAdSizeHeight90Banner or AdSize.BANNER_HEIGHT_90.
    BannerHeight90,

    /// kFBAdSizeInterstitial or AdSize.INTERSTITIAL.
    Interstitial,

    /// kFBAdSizeHeight250Rectangle or AdSize.RECTANGLE_HEIGHT_250.
    RectangleHeight250,
};

class FacebookAds final {
public:
    using AdCallback = std::function<void(const std::string& message)>;

    FacebookAds();
    ~FacebookAds();

    /// Gets the current device's hash, iOS only, for Android search for
    /// AdSettings tag.
    std::string getTestDeviceHash() const;

    /// Adds a test device.
    /// @param[in] hash The device's hash.
    void addTestDevice(const std::string& hash);

    /// Clears all test devices.
    void clearTestDevices();

    std::shared_ptr<AdViewInterface>
    createBannerAd(const std::string& adId, FacebookBannerAdSize adSize);

    std::shared_ptr<AdViewInterface>
    createNativeAd(const NativeAdBuilder& builder);

    std::shared_ptr<InterstitialAdInterface>
    createInterstitialAd(const std::string& placementId);

private:
    friend BannerAd;
    friend NativeAd;
    friend InterstitialAd;

    bool destroyBannerAd(const std::string& adId);
    bool destroyNativeAd(const std::string& adId);
    bool destroyInterstitialAd(const std::string& placementId);
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
