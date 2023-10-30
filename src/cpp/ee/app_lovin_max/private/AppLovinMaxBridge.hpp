#ifndef EE_X_APP_LOVIN_MAX_BRIDGE_HPP
#define EE_X_APP_LOVIN_MAX_BRIDGE_HPP

#include <ee/ads/IAd.hpp>
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

    virtual Task<bool> initialize(
        const std::string& bannerAdId,
        const std::string& rewardedAdId,
        const std::string& interstitialAdId) override;

    void addAnalytics(std::shared_ptr<ILibraryAnalytics> analytics) override;

    virtual std::shared_ptr<IBannerAd> createBannerAd(const std::string& adId, MaxBannerAdSize adSize) override;
    virtual std::shared_ptr<IFullScreenAd> createRewardedAd(const std::string& adId) override;
    virtual std::shared_ptr<IFullScreenAd> createInterstitialAd(const std::string& adId) override;

private:
    friend InterstitialAd;
    friend RewardedAd;

    std::pair<int, int> getBannerAdSize(BannerAdSize adSize);

    bool destroyBannerAd(const std::string& adId);
    bool destroyInterstitialAd(const std::string& adId);
    bool destroyRewardedAd(const std::string& adId);

    void onBannerAdPaid(const std::string& jsonStr);

    bool hasInterstitialAd() const;
    void loadInterstitialAd();
    void showInterstitialAd(const std::string& adId);

    bool hasRewardedAd() const;
    Task<bool> loadRewardedAd();
    void showRewardedAd(const std::string& adId);

    void onInterstitialAdLoaded();
    void onInterstitialAdFailedToLoad(int code, const std::string& message);
    void onInterstitialAdFailedToShow(int code, const std::string& message);
    void onInterstitialAdClicked();
    void onInterstitialAdClosed();
    void onInterstitialAdPaid(const std::string& jsonStr);

    void onRewardedAdLoaded();
    void onRewardedAdFailedToLoad(int code, const std::string& message);
    void onRewardedAdFailedToShow(int code, const std::string& message);
    void onRewardedAdClicked();
    void onRewardedAdClosed(bool rewarded);
    void onRewardedAdPaid(const std::string& jsonStr);

    void onAdPaid(AdFormat adFormat, const std::string& jsonStr);

    void onMediationAdClosed(AdResult result);

    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
    std::string network_;

    /// Share the same ad instance.
    std::shared_ptr<BannerAd> bannerAd_;
    std::shared_ptr<IBannerAd> sharedBannerAd_;
    std::shared_ptr<InterstitialAd> interstitialAd_;
    std::shared_ptr<IFullScreenAd> sharedInterstitialAd_;
    std::shared_ptr<RewardedAd> rewardedAd_;
    std::shared_ptr<IFullScreenAd> sharedRewardedAd_;

    std::shared_ptr<ads::IAsyncHelper<AdResult>> displayer_;
    std::shared_ptr<ILibraryAnalytics> analytics_;
};
} // namespace app_lovin_max
} // namespace ee

#endif /* EE_X_APP_LOVIN_MAX_BRIDGE_HPP */
