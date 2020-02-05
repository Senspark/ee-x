//
//  AppLovinRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/app_lovin/private/AppLovinRewardedVideo.hpp"

#include "ee/app_lovin/AppLovinBridge.hpp"

namespace ee {
namespace app_lovin {
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
} // namespace app_lovin
} // namespace ee
