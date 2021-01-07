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

    virtual Task<bool> initialize(const std::string& appKey) override;
    virtual std::shared_ptr<IFullScreenAd>
    createInterstitialAd(const std::string& adId) override;
    virtual std::shared_ptr<IFullScreenAd>
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

    void onMediationAdClosed(FullScreenAdResult result);

    IMessageBridge& bridge_;
    const Logger& logger_;

    /// Share the same ad instance.
    std::shared_ptr<InterstitialAd> interstitialAd_;
    std::shared_ptr<IFullScreenAd> sharedInterstitialAd_;
    std::shared_ptr<RewardedAd> rewardedAd_;
    std::shared_ptr<IFullScreenAd> sharedRewardedAd_;

    std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>> displayer_;
};
} // namespace iron_source
} // namespace ee

#endif /* EE_X_IRON_SOURCE_BRIDGE_HPP */
