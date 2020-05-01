//
//  AdMob.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_ADMOB_BRIDGE_HPP
#define EE_X_ADMOB_BRIDGE_HPP

#include <map>
#include <memory>
#include <string>

#include "ee/AdMobFwd.hpp"

namespace ee {
namespace admob {
class Bridge final {
public:
    Bridge();
    ~Bridge();

    /// Constructs an AdMob bridge with a custom logger.
    explicit Bridge(const Logger& logger);

    /// Optional.
    /// Calls this method for faster ads loading.
    /// https://groups.google.com/forum/#!topic/google-admob-ads-sdk/N5okpElFHZs
    /// https://groups.google.com/forum/#!topic/google-admob-ads-sdk/kayQ3VZJqkU
    /// @param[in] applicationId The AdMob application code.
    void initialize(const std::string& applicationId);

    /// kGADSimulatorID or AdRequest.DEVICE_ID_EMULATOR.
    std::string getEmulatorTestDeviceHash() const;

    void addTestDevice(const std::string& hash);

    /// Creates a banner ad.
    /// @param[in] adId The banner ad unit ID.
    /// @param[in] adSize The banner ad size.
    /// @return A reference to the banner ad if it created successfully, null
    /// otherwise.
    std::shared_ptr<IAdView> createBannerAd(const std::string& adId,
                                            BannerAdSize adSize);

    std::shared_ptr<IAdView> createNativeAd(const std::string& adId,
                                            const std::string& layoutName,
                                            const NativeAdLayout& identifiers);

    /// Creates an interstitial ad.
    /// @param[in] adId The ad unit ID.
    std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId);

    /// Creates an rewarded ad.
    /// @param[in] adId The ad unit ID.
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
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_BRIDGE_HPP */
