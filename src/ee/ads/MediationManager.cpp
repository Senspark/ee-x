
//
//  MediationManager.cpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#include "ee/ads/MediationManager.hpp"

#include <mutex>

#include "ee/Core.hpp"
#include "ee/ads/IInterstitialAd.hpp"
#include "ee/ads/IRewardedVideo.hpp"

namespace ee {
namespace ads {
using Self = MediationManager;

Self& Self::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

Self::MediationManager() {
    interstitialAdDestroyed_ = false;
    rewardedVideoDestroyed_ = false;
}

Self::~MediationManager() {}

bool Self::startInterstitialAd(const OnInterstitialCloseCallback& callback) {
    return registerInterstitialAd(callback);
}

bool Self::finishInterstitialAd() {
    return deregisterInterstitialAd(false);
}

bool Self::destroyInterstitialAd() {
    return deregisterInterstitialAd(true);
}

bool Self::registerInterstitialAd(const OnInterstitialCloseCallback& callback) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (_onInterstitialCloseCallback != nullptr) {
        return false;
    }

    _onInterstitialCloseCallback = callback;
    interstitialAdDestroyed_ = false;
    return true;
}

bool Self::deregisterInterstitialAd(bool destroyed) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (_onInterstitialCloseCallback == nullptr) {
        if (interstitialAdDestroyed_) {
            return true;
        }
        return false;
    }

    _onInterstitialCloseCallback();
    _onInterstitialCloseCallback = nullptr;
    interstitialAdDestroyed_ = destroyed;
    return true;
}

bool Self::setInterstitialAdDone() {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (_onInterstitialCloseCallback == nullptr) {
        if (interstitialAdDestroyed_) {
            return true;
        }
        return false;
    }

    _onInterstitialCloseCallback();
    _onInterstitialCloseCallback = nullptr;
    return true;
}

bool Self::startRewardedVideo(const OnVideoCloseCallback& callback) {
    return registerRewardedVideo(callback);
}

bool Self::finishRewardedVideo(bool rewarded) {
    return deregisterRewardedVideo(false, rewarded);
}

bool Self::destroyRewardedVideo() {
    return deregisterRewardedVideo(true, false);
}

bool Self::registerRewardedVideo(const OnVideoCloseCallback& callback) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (_onVideoCloseCallback != nullptr) {
        return false;
    }
    _onVideoCloseCallback = callback;
    rewardedVideoDestroyed_ = false;
    return true;
}

bool Self::deregisterRewardedVideo(bool destroyed, bool rewarded) {
    std::lock_guard<core::SpinLock> guard(locker_);
    if (_onVideoCloseCallback == nullptr) {
        if (rewardedVideoDestroyed_) {
            return true;
        }
        return false;
    }

    _onVideoCloseCallback(rewarded);
    _onVideoCloseCallback = nullptr;

    rewardedVideoDestroyed_ = destroyed;
    return true;
}
} // namespace ads
} // namespace ee
