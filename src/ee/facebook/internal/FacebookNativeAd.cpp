//
//  FacebookNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include <cassert>

#include "ee/core/internal/MessageBridge.hpp"
#include "ee/facebook/FacebookAdsBridge.hpp"
#include "ee/facebook/internal/FacebookNativeAd.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = NativeAd;

Self::NativeAd(FacebookAds* plugin, const std::string& adId)
    : Super()
    , adId_(adId)
    , plugin_(plugin)
    , helper_("FacebookNativeAd", adId)
    , bridgeHelper_(helper_) {}

Self::~NativeAd() {
    bool succeeded = plugin_->destroyNativeAd(adId_);
    assert(succeeded);
}

bool Self::isLoaded() const {
    return bridgeHelper_.isLoaded();
}

void Self::load() {
    bridgeHelper_.load();
}

std::pair<int, int> Self::getPosition() const {
    return bridgeHelper_.getPosition();
}

void Self::setPosition(int x, int y) {
    bridgeHelper_.setPosition(x, y);
}

std::pair<int, int> Self::getSize() const {
    return bridgeHelper_.getSize();
}

void Self::setSize(int width, int height) {
    bridgeHelper_.setSize(width, height);
}

void Self::setVisible(bool visible) {
    bridgeHelper_.setVisible(visible);
}
} // namespace facebook
} // namespace ee
