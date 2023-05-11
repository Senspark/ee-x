#ifndef EE_X_APP_LOVIN_MAX_BRIDGE_HPP
#define EE_X_APP_LOVIN_MAX_BRIDGE_HPP

#include "ee/app_lovin_max/IAppLovinMaxBridge.hpp"

namespace ee {
namespace app_lovin_max {
class Bridge final : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
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
    void onInterstitialAdFailedToLoad(int code, const std::string& message);
    void onInterstitialAdClicked();
    void onInterstitialAdClosed();

    void onRewardedAdLoaded();
    void onRewardedAdFailedToLoad(int code, const std::string& message);
    void onRewardedAdClicked();
    void onRewardedAdClosed(bool rewarded);

    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
    std::string network_;

    /// Share the same instance.
    RewardedAd* rewardedAd_;
    std::shared_ptr<IFullScreenAd> sharedRewardedAd_;

    std::shared_ptr<ads::IAsyncHelper<AdResult>> displayer_;
};
} // namespace app_lovin_max
} // namespace ee

#endif /* EE_X_APP_LOVIN_MAX_BRIDGE_HPP */
