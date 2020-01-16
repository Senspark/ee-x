//
//  IInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include "ee/ads/IInterstitialAd.hpp"

#include "ee/ads/MediationManager.hpp"

namespace ee {
namespace ads {
using Self = IInterstitialAd;

Self::IInterstitialAd() {
    callback_ = nullptr;
}

Self::~IInterstitialAd() {
    auto&& mediation = MediationManager::getInstance();
    mediation.destroyInterstitialAd();
}

void Self::setResultCallback(const InterstitialAdCallback& callback) {
    callback_ = callback;
}

void Self::setDone() {
    if (callback_) {
        callback_();
    }
}

void Self::setOnClickedCallback(const OnClickedCallback& callback) {
    onClickCallback_ = callback;
}

void Self::doOnClicked() {
    performClick();
}

void Self::performClick() {
    if (onClickCallback_) {
        onClickCallback_();
    }
}
} // namespace ads
} // namespace ee
