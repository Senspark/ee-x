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
#include <map>
#include <memory>
#include <string>

#include "ee/CoreFwd.hpp"
#include "ee/FacebookAdsFwd.hpp"
#include "ee/ads/IAdView.hpp"
#include "ee/ads/IInterstitialAd.hpp"
#include "ee/ads/IRewardedVideo.hpp"
#include "ee/facebookads/FacebookBannerAdSize.hpp"

namespace ee {
namespace facebook {
class FacebookAds final {
public:
    FacebookAds();
    ~FacebookAds();

    explicit FacebookAds(const Logger& logger);

    /// Gets the current device's hash, iOS only, for Android search for
    /// AdSettings tag.
    std::string getTestDeviceHash() const;

    /// Adds a test device.
    /// @param[in] hash The device's hash.
    void addTestDevice(const std::string& hash);

    /// Clears all test devices.
    void clearTestDevices();

    /// Creates a banner ad.
    /// @param[in] adId The banner ad ID.
    /// @param[in] adSize The banner ad predefined size.
    std::shared_ptr<IAdView> createBannerAd(const std::string& adId,
                                            BannerAdSize adSize);

    /// Creates a native ad.
    /// @param[in] adId The native ad ID.
    /// @param[in] layoutName The layout name (.xml for Android, .xib for iOS).
    /// @param[in] identifiers Android only.
    std::shared_ptr<IAdView> createNativeAd(const std::string& adId,
                                            const std::string& layoutName,
                                            const NativeAdLayout& identifiers);

    /// Creates an interstitial ad.
    /// @param[in] placementId The ad placement ID>
    std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& placementId);

    std::shared_ptr<IRewardedVideo>
    createRewardedVideo(const std::string& placementId);

private:
    friend BannerAd;
    friend NativeAd;
    friend InterstitialAd;
    friend RewardedVideo;

    bool destroyBannerAd(const std::string& adId);
    bool destroyNativeAd(const std::string& adId);
    bool destroyInterstitialAd(const std::string& placementId);
    bool destroyRewardVideoAd(const std::string& placementId);

    std::map<std::string, RewardedVideo*> rewardedVideos_;

    IMessageBridge& bridge_;
    const Logger& logger_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
