//
//  MediationManager.hpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#ifndef EE_X_ADS_MEDIATION_MANAGER_HPP
#define EE_X_ADS_MEDIATION_MANAGER_HPP

#include "ee/AdsFwd.hpp"
#include "ee/core/internal/SpinLock.hpp"

namespace ee {
namespace ads {
/// Fix issue where AdMob consumes other ads' callbacks.
class MediationManager {
private:
    using Self = MediationManager;

public:
    static Self& getInstance();

    bool registerInterstitialAd(InterstitialAdInterface* ad);
    bool deregisterInterstitialAd(InterstitialAdInterface* ad);
    bool setInterstitialAdDone();

    bool registerRewardedVideo(RewardedVideoInterface* ad);
    bool deregisterRewardedVideo(RewardedVideoInterface* ad);
    bool setRewardedVideoResult(bool result);

protected:
    MediationManager();
    ~MediationManager();

    MediationManager(const Self&) = delete;
    Self& operator=(const Self&) = delete;

private:
    InterstitialAdInterface* interstitialAd_;
    RewardedVideoInterface* rewardedVideo_;
    core::SpinLock locker_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_ADS_MEDIATION_MANAGER_HPP */
