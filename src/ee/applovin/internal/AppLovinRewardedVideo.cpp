//
//  AppLovinRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/applovin/internal/AppLovinRewardedVideo.hpp"

#include "ee/applovin/AppLovinBridge.hpp"

namespace ee {
namespace applovin {
using Self = RewardedVideo;

Self::RewardedVideo(const Logger& logger, AppLovin* plugin)
    : Super(logger) {
    plugin_ = plugin;
}

Self::~RewardedVideo() {
    plugin_->destroyRewardedVideo();
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedVideo();
}

void Self::load() {
    plugin_->loadRewardedVideo();
}

bool Self::show() {
    return plugin_->showRewardedVideo();
}
} // namespace applovin
} // namespace ee
