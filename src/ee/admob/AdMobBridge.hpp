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
#include <unordered_map>

#include "ee/AdMobFwd.hpp"
#include "ee/CoreFwd.hpp"
#include "ee/admob/AdMobBannerAdSize.hpp"
#include "ee/ads/IAdView.hpp"
#include "ee/ads/IInterstitialAd.hpp"
#include "ee/ads/IRewardedVideo.hpp"

namespace ee {
namespace admob {
class AdMob final {
public:
    AdMob();
    ~AdMob();
    
    /// Constructs an AdMob bridge with a custom logger.
    explicit AdMob(const Logger& logger);

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

    std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId);

    std::shared_ptr<IRewardedVideo>
    createRewardedVideo(const std::string& adId);

private:
    friend BannerAd;
    friend NativeAd;
    friend InterstitialAd;
    friend RewardedVideo;

    bool destroyBannerAd(const std::string& adId);
    bool destroyNativeAd(const std::string& adId);
    bool destroyInterstitialAd(const std::string& adId);

    bool destroyRewardedVideo(const std::string& adId);
    bool hasRewardedVideo() const;
    void loadRewardedVideo(const std::string& adId);
    bool showRewardedVideo();

    void onLoaded();
    void onFailedToLoad(const std::string& message);
    void onReward();
    void onOpened();
    void onClosed();

    bool loading_;
    bool rewarded_;
    std::string currentId_;

    IMessageBridge& bridge_;
    const Logger& logger_;
    std::map<std::string, RewardedVideo*> rewardedVideos_;
};
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_BRIDGE_HPP */
