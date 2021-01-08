//
//  ALovinAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_APP_LOVIN_BRIDGE_HPP
#define EE_X_APP_LOVIN_BRIDGE_HPP

#include <string>

#include "ee/app_lovin/IAppLovinBridge.hpp"

namespace ee {
namespace app_lovin {
class Bridge final : public IBridge {
public:
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual Task<bool> initialize(const std::string& key) override;
    virtual void setVerboseLogging(bool enabled) override;
    virtual void setMuted(bool enabled) override;
    virtual std::shared_ptr<IFullScreenAd> createRewardedAd() override;

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
    std::shared_ptr<IFullScreenAd> sharedRewardedAd_;

    std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>> displayer_;
};
} // namespace app_lovin
} // namespace ee

#endif /* EE_X_APP_LOVIN_BRIDGE_HPP */
