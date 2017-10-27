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
    : helper_(helper) {
    anchorX_ = 0.0f;
    anchorY_ = 0.0f;
}

bool Self::isLoaded() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(helper_.k__isLoaded());
    return core::toBool(response);
}

void Self::load() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(helper_.k__load());
}

std::pair<float, float> Self::getAnchor() const {
    return std::tie(anchorX_, anchorY_);
}

void Self::setAnchor(float x, float y) {
    int width, height;
    std::tie(width, height) = getSize();
    int positionX, positionY;
    std::tie(positionX, positionY) = getPositionTopLeft();
    setPositionTopLeft(positionX - static_cast<int>((x - anchorX_) * width),
                       positionY - static_cast<int>((y - anchorY_) * height));
    anchorX_ = x;
    anchorY_ = y;
}

std::pair<int, int> Self::getPosition() const {
    int width, height;
    std::tie(width, height) = getSize();
    float anchorX, anchorY;
    std::tie(anchorX, anchorY) = getAnchor();
    int x, y;
    std::tie(x, y) = getPositionTopLeft();
    return std::make_pair(x + anchorX * width, y + anchorY * height);
}

void Self::setPosition(int x, int y) {
    int width, height;
    std::tie(width, height) = getSize();
    float anchorX, anchorY;
    std::tie(anchorX, anchorY) = getAnchor();
    setPositionTopLeft(x - static_cast<int>(anchorX * width),
                       y - static_cast<int>(anchorY * height));
}

std::pair<int, int> Self::getPositionTopLeft() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(helper_.k__getPosition());
    auto json = nlohmann::json::parse(response);
    auto x = json["x"].get<int>();
    auto y = json["y"].get<int>();
    return std::make_pair(x, y);
}

void Self::setPositionTopLeft(int x, int y) {
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
    int currentWidth, currentHeight;
    std::tie(currentWidth, currentHeight) = getSize();
    float anchorX, anchorY;
    std::tie(anchorX, anchorY) = getAnchor();
    int x, y;
    std::tie(x, y) = getPositionTopLeft();
    setPositionTopLeft(
        x - static_cast<int>((width - currentWidth) * anchorX),
        y - static_cast<int>((height - currentHeight) * anchorY));

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
