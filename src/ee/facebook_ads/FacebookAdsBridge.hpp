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

#include "ee/FacebookAdsFwd.hpp"

namespace ee {
namespace facebook_ads {
class Bridge final {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

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
    /// @param[in] adId The ad placement ID>
    std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId);

    std::shared_ptr<IRewardedAd> createRewardedAd(const std::string& adId);

private:
    friend BannerAd;
    friend NativeAd;
    friend InterstitialAd;
    friend RewardedAd;

    std::pair<int, int> getBannerAdSize(BannerAdSize adSize);

    bool destroyBannerAd(const std::string& adId);
    bool destroyNativeAd(const std::string& adId);
    bool destroyInterstitialAd(const std::string& adId);
    bool destroyRewardedAd(const std::string& adId);

    IMessageBridge& bridge_;
    const Logger& logger_;

    std::map<std::string, std::shared_ptr<IAdView>> bannerAds_;
    std::map<std::string, std::shared_ptr<IAdView>> nativeAds_;
    std::map<std::string, std::shared_ptr<IInterstitialAd>> interstitialAds_;
    std::map<std::string, std::shared_ptr<IRewardedAd>> rewardedAds_;

    std::shared_ptr<ads::IAsyncHelper<bool>> interstitialAdDisplayer_;
    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> rewardedAdDisplayer_;
};
} // namespace facebook_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
