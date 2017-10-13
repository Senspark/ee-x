//
//  AdMobRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#include "AdMobRewardedVideo.hpp"
#include "ee/admob/AdMobBridge.hpp"

namespace ee {
namespace admob {
using Self = RewardedVideo;

Self::RewardedVideo(AdMob* plugin, const std::string& adId) {
    plugin_ = plugin;
    adId_ = adId;
}

Self::~RewardedVideo() {
    plugin_->destroyRewardedVideo(adId_);
}

bool Self::isLoaded() const {
    return adId_ == plugin_->currentId_ && plugin_->hasRewardedVideo();
}

void Self::load() {
    plugin_->loadRewardedVideo(adId_);
}

bool Self::show() {
    return adId_ == plugin_->currentId_ && plugin_->showRewardedVideo();
}
} // namespace admob
} // namespace ee
