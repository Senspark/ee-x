//
//  AdViewBridgeHelper.cpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#include "ee/ads/internal/AdViewBridgeHelper.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace ads {
using Self = AdViewBridgeHelper;

Self::AdViewBridgeHelper(const AdViewHelper& helper)
    : helper_(helper) {}

bool Self::isLoaded() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(helper_.k__isLoaded());
    return core::toBool(response);
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
    bridge.call(helper_.k__setVisible(), core::toString(visible));
}
} // namespace ads
} // namespace ee
