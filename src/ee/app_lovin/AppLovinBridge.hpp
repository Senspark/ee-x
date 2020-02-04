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

#include "ee/AppLovinFwd.hpp"

namespace ee {
namespace app_lovin {
class Bridge final {
public:
    Bridge();
    ~Bridge();
    
    explicit Bridge(const Logger& logger);

    void initialize(const std::string& key);

    /// Disabled by default.
    void setTestAdsEnabled(bool enabled);

    /// Disabled by default.
    void setVerboseLogging(bool enabled);

    /// Disabled by default.
    void setMuted(bool enabled);

    std::shared_ptr<IRewardedVideo> createRewardedVideo();

private:
    friend RewardedVideo;

    bool hasInterstitialAd() const;
    bool showInterstitialAd();

    bool destroyRewardedVideo();
    void loadRewardedVideo();
    bool hasRewardedVideo() const;
    bool showRewardedVideo();

    void onInterstitialAdHidden();
    void onRewardedVideoFailed(int errorCode);
    void onRewardedVideoDisplayed();
    void onRewardedVideoHidden();
    void onUserRewardVerified();

    bool verified_;
    bool errored_;
    RewardedVideo* rewardedVideo_;
    IMessageBridge& bridge_;
    const Logger& logger_;
};
} // namespace app_lovin
} // namespace ee

#endif /* EE_X_APP_LOVIN_BRIDGE_HPP */
