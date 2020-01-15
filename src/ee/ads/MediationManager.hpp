//
//  MediationManager.hpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#ifndef EE_X_ADS_MEDIATION_MANAGER_HPP
#define EE_X_ADS_MEDIATION_MANAGER_HPP

#include <functional>

#include <ee/core/internal/SpinLock.hpp>

#include "ee/AdsFwd.hpp"

namespace ee {
namespace ads {
/// Fix issue where AdMob consumes other ads' callbacks.
using OnInterstitialCloseCallback = std::function<void()>;
using OnVideoCloseCallback = std::function<void(bool rewarded)>;

class MediationManager {
private:
    using Self = MediationManager;

public:
    static Self& getInstance();

    bool startInterstitialAd(const OnInterstitialCloseCallback& callback);
    bool finishInterstitialAd();
    bool destroyInterstitialAd();
    bool setInterstitialAdDone();

    bool startRewardedVideo(const OnVideoCloseCallback& callback);
    bool finishRewardedVideo(bool rewarded);
    bool destroyRewardedVideo();

protected:
    MediationManager();
    ~MediationManager();

    MediationManager(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    bool registerInterstitialAd(const OnInterstitialCloseCallback& callback);
    bool deregisterInterstitialAd(bool destroyed);

    bool registerRewardedVideo(const OnVideoCloseCallback& callback);
    bool deregisterRewardedVideo(bool destroyed, bool rewarded);

private:
    OnInterstitialCloseCallback _onInterstitialCloseCallback;
    OnVideoCloseCallback _onVideoCloseCallback;
    bool interstitialAdDestroyed_;
    bool rewardedVideoDestroyed_;
    core::SpinLock locker_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_ADS_MEDIATION_MANAGER_HPP */
