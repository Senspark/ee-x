#ifndef EE_X_IRON_SOURCE_BRIDGE_HPP
#define EE_X_IRON_SOURCE_BRIDGE_HPP

#include <map>

#include "ee/iron_source/IIronSourceBridge.hpp"

namespace ee {
namespace iron_source {
class Bridge final : public IBridge {
public:
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;

    /// Initializes ironSource with the specified game ID.
    virtual void initialize(const std::string& appKey) override;

    /// Creates an interstitial ad with the specified placement ID.
    virtual std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId) override;

    /// Creates a rewarded vided with the specifie placement ID.
    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) override;

private:
    friend InterstitialAd;
    friend RewardedAd;

    bool destroyInterstitialAd(const std::string& adId);
    bool destroyRewardedAd(const std::string& adId);

    bool hasInterstitialAd() const;
    void loadInterstitialAd();
    void showInterstitialAd(const std::string& adId);

    bool hasRewardedAd() const;
    void showRewardedAd(const std::string& adId);

    void onInterstitialAdLoaded();
    void onInterstitialAdFailedToLoad(const std::string& message);
    void onInterstitialAdFailedToShow(const std::string& message);
    void onInterstitialAdClicked();
    void onInterstitialAdClosed();

    void onRewardedAdLoaded();
    void onRewardedAdFailedToShow(const std::string& message);
    void onRewardedAdClicked();
    void onRewardedAdClosed(bool rewarded);

    void onMediationAdClosed(bool rewarded);

    IMessageBridge& bridge_;
    const Logger& logger_;

    /// Share the same ad instance.
    InterstitialAd* interstitialAd_;
    std::shared_ptr<IInterstitialAd> sharedInterstitialAd_;
    RewardedAd* rewardedAd_;
    std::shared_ptr<IRewardedAd> sharedRewardedAd_;

    std::shared_ptr<ads::IAsyncHelper<bool>> interstitialAdDisplayer_;
    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> rewardedAdDisplayer_;
};
} // namespace iron_source
} // namespace ee

#endif /* EE_X_IRON_SOURCE_BRIDGE_HPP */
