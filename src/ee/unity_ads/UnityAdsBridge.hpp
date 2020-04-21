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
#include <memory>
#include <string>

#include "ee/UnityAdsFwd.hpp"

namespace ee {
namespace unity_ads {
class Bridge final {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    void destroy();

    /// Initializes Unity Ads.
    /// @param[in] gameId Unity Ads game ID.
    /// @param[in] testModeEnabled Whether test mode is enabled.
    void initialize(const std::string& gameId, bool testModeEnabled);

    /// Sets debug mode enabled.
    void setDebugModeEnabled(bool enabled);

    /// Creates an interstitial ad with the specified placement ID.
    std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId);

    /// Creates a rewarded video with the specified placement ID.
    std::shared_ptr<IRewardedAd> createRewardedAd(const std::string& adId);

private:
    friend InterstitialAd;
    friend RewardedAd;

    bool destroyInterstitialAd(const std::string& adId);
    bool destroyRewardedAd(const std::string& adId);

    bool hasRewardedAd(const std::string& adId) const;
    void showRewardedAd(const std::string& adId);

    void onLoaded(const std::string& adId);
    void onFailedToShow(const std::string& adId, const std::string& message);
    void onClosed(const std::string& adId, bool rewarded);

    void onMediationAdFailedToShow(const std::string& adId,
                                   const std::string& message);
    void onMediationAdClosed(const std::string& adId, bool rewarded);

    IMessageBridge& bridge_;
    const Logger& logger_;

    bool displaying_;

    /// Currently displaying ad ID.
    std::string adId_;

    /// Unity only has rewarded ads.
    std::map<std::string, std::shared_ptr<InterstitialAd>> interstitialAds_;
    std::map<std::string, std::shared_ptr<RewardedAd>> rewardedAds_;

    std::shared_ptr<ads::IAsyncHelper<bool>> interstitialAdDisplayer_;
    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> rewardedAdDisplayer_;
};
} // namespace unity_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
