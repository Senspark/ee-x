//
//  MediationManager.cpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#include <mutex>

#include "ee/ads/InterstitialAdInterface.hpp"
#include "ee/ads/RewardedVideoInterface.hpp"
#include "ee/ads/internal/MediationManager.hpp"

namespace ee {
namespace ads {
using Self = MediationManager;

Self& Self::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

Self::MediationManager() {
    interstitialAd_ = nullptr;
    rewardedVideo_ = nullptr;
}

Self::~MediationManager() {
    interstitialAd_ = nullptr;
    rewardedVideo_ = nullptr;
}

bool Self::registerInterstitialAd(InterstitialAdInterface* ad) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (interstitialAd_ != nullptr) {
        return false;
    }
    interstitialAd_ = ad;
    return true;
}

bool Self::deregisterInterstitialAd(InterstitialAdInterface* ad) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (interstitialAd_ == nullptr) {
        return false;
    }
    if (interstitialAd_ != ad) {
        return false;
    }
    interstitialAd_ = nullptr;
    return true;
}

bool Self::setInterstitialAdDone() {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (interstitialAd_ == nullptr) {
        return false;
    }
    interstitialAd_->setDone();
    interstitialAd_ = nullptr;
    return true;
}

bool Self::registerRewardedVideo(RewardedVideoInterface* ad) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (rewardedVideo_ != nullptr) {
        return false;
    }
    rewardedVideo_ = ad;
    return true;
}

bool Self::deregisterRewardedVideo(RewardedVideoInterface* ad) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (rewardedVideo_ == nullptr) {
        return false;
    }
    if (rewardedVideo_ != ad) {
        return false;
    }
    rewardedVideo_ = nullptr;
    return true;
}

bool Self::setRewardedVideoResult(bool result) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (rewardedVideo_ == nullptr) {
        return false;
    }
    rewardedVideo_->setResult(result);
    rewardedVideo_ = nullptr;
    return true;
}
} // namespace ads
} // namespace ee
