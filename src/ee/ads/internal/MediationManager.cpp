//
//  MediationManager.cpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#include <mutex>

#include "ee/Core.hpp"
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
    interstitialAdDestroyed_ = false;
    rewardedVideo_ = nullptr;
    rewardedVideoDestroyed_ = false;
}

Self::~MediationManager() {
    interstitialAd_ = nullptr;
    rewardedVideo_ = nullptr;
}

bool Self::startInterstitialAd(InterstitialAdInterface* ad) {
    return registerInterstitialAd(ad);
}

bool Self::finishInterstitialAd(InterstitialAdInterface* ad) {
    return deregisterInterstitialAd(ad, false);
}

bool Self::destroyInterstitialAd(InterstitialAdInterface* ad) {
    return deregisterInterstitialAd(ad, true);
}

bool Self::registerInterstitialAd(InterstitialAdInterface* ad) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (interstitialAd_ != nullptr) {
        return false;
    }
    interstitialAd_ = ad;
    interstitialAdDestroyed_ = false;
    return true;
}

bool Self::deregisterInterstitialAd(InterstitialAdInterface* ad,
                                    bool destroyed) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (interstitialAd_ == nullptr) {
        if (interstitialAdDestroyed_) {
            return true;
        }
        return false;
    }
    if (interstitialAd_ != ad) {
        return false;
    }
    interstitialAd_ = nullptr;
    interstitialAdDestroyed_ = destroyed;
    return true;
}

bool Self::setInterstitialAdDone() {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (interstitialAd_ == nullptr) {
        if (interstitialAdDestroyed_) {
            return true;
        }
        return false;
    }
    interstitialAd_->setDone();
    interstitialAd_ = nullptr;
    return true;
}

bool Self::startRewardedVideo(RewardedVideoInterface* ad) {
    return registerRewardedVideo(ad);
}

bool Self::finishRewardedVideo(RewardedVideoInterface* ad) {
    return deregisterRewardedVideo(ad, false);
}

bool Self::destroyRewardedVideo(RewardedVideoInterface* ad) {
    return deregisterRewardedVideo(ad, true);
}

bool Self::registerRewardedVideo(RewardedVideoInterface* ad) {
    Logger::getSystemLogger().debug(
        "%s: ad = %p current_ad = %p destroyed = %s", __PRETTY_FUNCTION__, ad,
        rewardedVideo_, core::toString(rewardedVideoDestroyed_).c_str());
    std::lock_guard<core::SpinLock> guard(locker_);
    if (rewardedVideo_ != nullptr) {
        return false;
    }
    rewardedVideo_ = ad;
    rewardedVideoDestroyed_ = false;
    return true;
}

bool Self::deregisterRewardedVideo(RewardedVideoInterface* ad, bool destroyed) {
    Logger::getSystemLogger().debug(
        "%s: ad = %p destroyed = %s current_ad = %p destroyed = %s",
        __PRETTY_FUNCTION__, ad, core::toString(destroyed).c_str(),
        rewardedVideo_, core::toString(rewardedVideoDestroyed_).c_str());
    std::lock_guard<core::SpinLock> guard(locker_);
    if (rewardedVideo_ == nullptr) {
        if (rewardedVideoDestroyed_) {
            return true;
        }
        return false;
    }
    if (rewardedVideo_ != ad) {
        return false;
    }
    rewardedVideo_ = nullptr;
    rewardedVideoDestroyed_ = destroyed;
    return true;
}

bool Self::setRewardedVideoResult(bool result) {
    Logger::getSystemLogger().debug(
        "%s: result = %s current_ad = %p destroyed = %s", __PRETTY_FUNCTION__,
        core::toString(result).c_str(), rewardedVideo_,
        core::toString(rewardedVideoDestroyed_).c_str());
    std::lock_guard<core::SpinLock> guard(locker_);
    if (rewardedVideo_ == nullptr) {
        if (rewardedVideoDestroyed_) {
            return true;
        }
        return false;
    }
    rewardedVideo_->setResult(result);
    rewardedVideo_ = nullptr;
    return true;
}
} // namespace ads
} // namespace ee
