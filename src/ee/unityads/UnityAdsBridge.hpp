//
//  UnityAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_UNITY_ADS_BRIDGE_HPP
#define EE_X_UNITY_ADS_BRIDGE_HPP

#include <map>

#include "ee/CoreFwd.hpp"
#include "ee/UnityAdsFwd.hpp"
#include "ee/ads/IInterstitialAd.hpp"
#include "ee/ads/IRewardedVideo.hpp"

namespace ee {
namespace unityads {
class UnityAds final {
public:
    UnityAds();
    ~UnityAds();
    
    explicit UnityAds(Logger& logger);

    /// Initializes Unity Ads.
    /// @param[in] gameId Unity Ads game ID.
    /// @param[in] testModeEnabled Whether test mode is enabled.
    void initialize(const std::string& gameId, bool testModeEnabled);

    /// Sets debug mode enabled.
    void setDebugModeEnabled(bool enabled);

    /// Creates a rewarded video with the specified placement ID.
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

    bool isRewardedVideoReady(const std::string& placementId) const;
    bool showRewardedVideo(const std::string& placementId);

    void onError(const std::string& placementId);
    void onSkipped(const std::string& placementId);
    void onFinished(const std::string& placementId);
    void finish(const std::string& placementId, bool result);

    bool errored_;
    bool displayed_;

    IMessageBridge& bridge_;
    Logger& logger_;
    std::map<std::string, RewardedVideo*> rewardedVideos_;
    std::map<std::string, InterstitialAd*> interstitialAds_;
};
} // namespace unityads
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
