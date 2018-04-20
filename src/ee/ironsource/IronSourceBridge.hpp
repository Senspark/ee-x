#ifndef EE_X_IRON_SOURCE_BRIDGE_HPP
#define EE_X_IRON_SOURCE_BRIDGE_HPP

#include <map>

#include "ee/CoreFwd.hpp"
#include "ee/IronSourceFwd.hpp"
#include "ee/ads/IRewardedVideo.hpp"
#include "ee/ads/IInterstitialAd.hpp"

namespace ee {
namespace ironsource {
class IronSource final {
public:
    IronSource();
    ~IronSource();

    /// Initializes ironSource with the specified game ID.
    void initialize(const std::string& gameId);

    /// Creates a rewarded vided with the specifie placement ID.
    std::shared_ptr<IRewardedVideo>
    createRewardedVideo(const std::string& placementId);

    /// Creates an interstitial ad with the specified placement ID.
    std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& placementId);
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

    bool errored_;
    bool rewarded_;

    IMessageBridge& bridge_;
    std::map<std::string, RewardedVideo*> rewardedVideos_;
    std::map<std::string, InterstitialAd*> interstitialAds_;
};
} // namespace ironsource
} // namespace ee

#endif /* EE_X_IRON_SOURCE_BRIDGE_HPP */
