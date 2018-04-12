//
//  IRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/5/17.
//
//

#include "ee/ads/IRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Utils.hpp"

namespace ee {
namespace ads {
using Self = IRewardedVideo;

Self::IRewardedVideo() {
    callback_ = nullptr;
}

Self::~IRewardedVideo() {
    auto&& mediation = MediationManager::getInstance();
    mediation.destroyRewardedVideo();
}

void Self::setResultCallback(const RewardedVideoCallback& callback) {
    callback_ = callback;
}

void Self::setResult(bool result) {
    Logger::getSystemLogger().debug(
        "%s: this = %p result = %s has callback = %s", __PRETTY_FUNCTION__,
        this, core::toString(result).c_str(),
        core::toString(!!callback_).c_str());
    if (callback_) {
        callback_(result);
    }
}
} // namespace ads
} // namespace ee
