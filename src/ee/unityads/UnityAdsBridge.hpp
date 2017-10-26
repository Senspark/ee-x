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

#include "ee/UnityAdsFwd.hpp"
#include "ee/ads/InterstitialAdInterface.hpp"
#include "ee/ads/RewardedVideoInterface.hpp"

namespace ee {
namespace unityads {
class UnityAds final {
public:
    UnityAds();
    ~UnityAds();

    void initialize(const std::string& gameId, bool testModeEnabled);

    void setDebugModeEnabled(bool enabled);

    std::shared_ptr<RewardedVideoInterface>
    createRewardedVideo(const std::string& placementId);

    std::shared_ptr<InterstitialAdInterface>
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

    std::map<std::string, RewardedVideo*> rewardedVideos_;
    std::map<std::string, InterstitialAd*> interstitialAds_;
};
} // namespace unityads
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
