//
//  ALovinAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_APP_LOVIN_BRIDGE_HPP
#define EE_X_APP_LOVIN_BRIDGE_HPP

#ifdef __cplusplus

#include <string>

#include "ee/AppLovinFwd.hpp"

namespace ee {
namespace app_lovin {
class Bridge final {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    void destroy();

    void initialize(const std::string& key);

    /// Disabled by default.
    void setTestAdsEnabled(bool enabled);

    /// Disabled by default.
    void setVerboseLogging(bool enabled);

    /// Disabled by default.
    void setMuted(bool enabled);

    std::shared_ptr<IRewardedAd> createRewardedAd();

private:
    friend RewardedAd;

    bool destroyRewardedAd();

    bool hasInterstitialAd() const;
    void loadInterstitialAd();
    void showInterstitialAd();

    bool hasRewardedAd() const;
    void loadRewardedAd();
    void showRewardedAd();

    void onInterstitialAdLoaded();
    void onInterstitialAdFailedToLoad(const std::string& message);
    void onInterstitialAdClicked();
    void onInterstitialAdClosed();

    void onRewardedAdLoaded();
    void onRewardedAdFailedToLoad(const std::string& message);
    void onRewardedAdClicked();
    void onRewardedAdClosed(bool rewarded);

    IMessageBridge& bridge_;
    const Logger& logger_;

    /// Share the same instance.
    RewardedAd* rewardedAd_;
    std::shared_ptr<IRewardedAd> sharedRewardedAd_;

    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> rewardedAdDisplayer_;
};
} // namespace app_lovin
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_APP_LOVIN_BRIDGE_HPP */
