//
//  MediationManager.cpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#include <mutex>

#include "ee/Core.hpp"
#include "ee/ads/IInterstitialAd.hpp"
#include "ee/ads/IRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"

namespace ee {
namespace ads {
using Self = MediationManager;

Self& Self::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

Self::MediationManager() {
    interstitialAdDestroyed_ = false;
    rewardedVideo_ = nullptr;
    rewardedVideoDestroyed_ = false;
}

Self::~MediationManager() {
    
    rewardedVideo_ = nullptr;
}

bool Self::startInterstitialAd(const OnCloseCallback& callback) {
    
    return registerInterstitialAd(callback);
}

bool Self::finishInterstitialAd() {
    return deregisterInterstitialAd(false);
}

bool Self::destroyInterstitialAd() {
    return deregisterInterstitialAd(true);
}

bool Self::registerInterstitialAd(const OnCloseCallback& callback) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (_onCloseCallback != nullptr) {
        return false;
    }
    
    _onCloseCallback = callback;
    interstitialAdDestroyed_ = false;
    return true;
}

bool Self::deregisterInterstitialAd(bool destroyed) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (_onCloseCallback == nullptr) {
        if (interstitialAdDestroyed_) {
            return true;
        }
        return false;
    }
    
    _onCloseCallback();
    _onCloseCallback = nullptr;
    interstitialAdDestroyed_ = destroyed;
    return true;
}

bool Self::setInterstitialAdDone() {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (_onCloseCallback == nullptr) {
        if (interstitialAdDestroyed_) {
            return true;
        }
        return false;
    }
    
    _onCloseCallback();
    _onCloseCallback = nullptr;
    return true;
}

bool Self::startRewardedVideo(IRewardedVideo* ad) {
    return registerRewardedVideo(ad);
}

bool Self::finishRewardedVideo(IRewardedVideo* ad) {
    return deregisterRewardedVideo(ad, false);
}

bool Self::destroyRewardedVideo(IRewardedVideo* ad) {
    return deregisterRewardedVideo(ad, true);
}

bool Self::registerRewardedVideo(IRewardedVideo* ad) {
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

bool Self::deregisterRewardedVideo(IRewardedVideo* ad, bool destroyed) {
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
