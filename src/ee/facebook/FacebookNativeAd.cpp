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
#include "ee/facebook/FacebookNativeAd.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = FacebookNativeAd;

namespace {
auto k__isLoaded(const std::string& id) {
    return "FacebookNativeAd_isLoaded_" + id;
}

auto k__load(const std::string& id) {
    return "FacebookNativeAd_load_" + id;
}

auto k__getPosition(const std::string& id) {
    return "FacebookNativeAd_getPosition_" + id;
}

auto k__setPosition(const std::string& id) {
    return "FacebookNativeAd_setPosition_" + id;
}

auto k__getSize(const std::string& id) {
    return "FacebookNativeAd_getSize_" + id;
}

auto k__setSize(const std::string& id) {
    return "FacebookNativeAd_setSize_" + id;
}

auto k__setVisible(const std::string& id) {
    return "FacebookNativeAd_setVisible_" + id;
}
} // namespace

Self::FacebookNativeAd(FacebookAds* plugin, const std::string& adId)
    : Super(adId)
    , plugin_(plugin) {}

Self::~FacebookNativeAd() {
    bool succeeded = plugin_->destroyNativead(getAdId());
    assert(succeeded);
}

bool Self::isLoaded() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__isLoaded(getAdId()));
    return response == "true";
}

void Self::load() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__load(getAdId()));
}

std::pair<int, int> Self::getPosition() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__getPosition(getAdId()));
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
    bridge.call(k__setPosition(getAdId()), json.dump());
}

std::pair<int, int> Self::getSize() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__getSize(getAdId()));
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
    bridge.call(k__setSize(getAdId()), json.dump());
}

void Self::setVisible(bool visible) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__setVisible(getAdId()), visible ? "true" : "false");
}
} // namespace facebook
} // namespace ee
