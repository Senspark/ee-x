//
//  FacebookBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include <cassert>

#include "ee/core/internal/MessageBridge.hpp"
#include "ee/facebook/FacebookAdsBridge.hpp"
#include "ee/facebook/internal/FacebookBannerAd.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = BannerAd;

Self::BannerAd(FacebookAds* plugin, const std::string& adId)
    : Super(adId)
    , plugin_(plugin)
    , helper_("FacebookBannerAd", adId) {}

Self::~BannerAd() {
    bool succeeded = plugin_->destroyBannerAd(getAdId());
    assert(succeeded);
}

bool Self::isLoaded() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(helper_.k__isLoaded());
    return response == "true";
}

void Self::load() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(helper_.k__load());
}

std::pair<int, int> Self::getPosition() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(helper_.k__getPosition());
    auto json = nlohmann::json::parse(response);
    auto x = json["x"].get<int>();
    auto y = json["y"].get<int>();
    return std::make_pair(x, y);
}

void Self::setPosition(int x, int y) {
    nlohmann::json json;
    json["x"] = x;
    json["y"] = y;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(helper_.k__setPosition(), json.dump());
}

std::pair<int, int> Self::getSize() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(helper_.k__getSize());
    auto json = nlohmann::json::parse(response);
    auto width = json["width"].get<int>();
    auto height = json["height"].get<int>();
    return std::make_pair(width, height);
}

void Self::setSize(int width, int height) {
    nlohmann::json json;
    json["width"] = width;
    json["height"] = height;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(helper_.k__setSize(), json.dump());
}

void Self::setVisible(bool visible) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(helper_.k__setVisible(), visible ? "true" : "false");
}
} // namespace facebook
} // namespace ee
