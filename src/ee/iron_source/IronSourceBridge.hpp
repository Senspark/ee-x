#ifndef EE_X_IRON_SOURCE_BRIDGE_HPP
#define EE_X_IRON_SOURCE_BRIDGE_HPP

#include <map>
#include <memory>
#include <string>

#include "ee/IronSourceFwd.hpp"

namespace ee {
namespace iron_source {
class Bridge final {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    /// Initializes ironSource with the specified game ID.
    void initialize(const std::string& gameId);

    /// Creates a rewarded vided with the specifie placement ID.
    std::shared_ptr<IRewardedVideo>
    createRewardedVideo(const std::string& placementId);

    /// Creates an interstitial ad with the specified placement ID.
    std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& placementId);

    void setCloseTimeout(float timeout);

private:
    friend RewardedVideo;
    friend InterstitialAd;

    bool destroyRewardedVideo(const std::string& placementId);
    bool destroyInterstitialAd(const std::string& placementId);

    bool hasRewardedVideo() const;
    bool showRewardedVideo(const std::string& placementId);

    void loadInterstitial();
    bool hasInterstitial() const;
    bool showInterstitial(const std::string& placementId);

    void onRewarded(const std::string& placementId);
    void onFailed();
    void onOpened();
    void onClosed();
    void onRewardClicked();
    void doRewardAndFinishAds();

    void onInterstitialOpened();
    void onInterstitialFailed();
    void onInterstitialClosed();
    void onInterstitialClicked();

    IMessageBridge& bridge_;
    const Logger& logger_;
    std::map<std::string, RewardedVideo*> rewardedVideos_;
    std::map<std::string, InterstitialAd*> interstitialAds_;
    std::string placementId_;

    std::unique_ptr<core::SpinLock> handlerLock_;

    float _closeTimeout{1};
    bool rewarded_{false};
};
} // namespace iron_source
} // namespace ee

#endif /* EE_X_IRON_SOURCE_BRIDGE_HPP */
