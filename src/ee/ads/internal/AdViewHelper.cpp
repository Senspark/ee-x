//
//  AdViewHelper.cpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#include "ee/ads/internal/AdViewHelper.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/core/Utils.hpp>
#include <ee/core/internal/IMessageBridge.hpp>

namespace ee {
namespace ads {
using Self = AdViewHelper;

Self::AdViewHelper(IMessageBridge& bridge, const MessageHelper& helper)
    : bridge_(bridge)
    , helper_(helper)
    , anchorX_(0.0f)
    , anchorY_(0.0f) {}

bool Self::isLoaded() const {
    assert(isMainThread());
    auto response = bridge_.call(helper_.isLoaded());
    return core::toBool(response);
}

void Self::load() {
    assert(isMainThread());
    bridge_.call(helper_.load());
}

std::pair<float, float> Self::getAnchor() const {
    return std::pair(anchorX_, anchorY_);
}

void Self::setAnchor(float x, float y) {
    assert(isMainThread());
    auto [width, height] = getSize();
    auto [positionX, positionY] = getPositionTopLeft();
    setPositionTopLeft(positionX - static_cast<int>((x - anchorX_) * width),
                       positionY - static_cast<int>((y - anchorY_) * height));
    anchorX_ = x;
    anchorY_ = y;
}

std::pair<int, int> Self::getPosition() const {
    assert(isMainThread());
    auto [width, height] = getSize();
    auto [anchorX, anchorY] = getAnchor();
    auto [x, y] = getPositionTopLeft();
    return std::pair(x + anchorX * width, y + anchorY * height);
}

void Self::setPosition(int x, int y) {
    assert(isMainThread());
    auto [width, height] = getSize();
    auto [anchorX, anchorY] = getAnchor();
    setPositionTopLeft(x - static_cast<int>(anchorX * width),
                       y - static_cast<int>(anchorY * height));
}

std::pair<int, int> Self::getPositionTopLeft() const {
    assert(isMainThread());
    auto response = bridge_.call(helper_.getPosition());
    auto json = nlohmann::json::parse(response);
    auto x = json["x"].get<int>();
    auto y = json["y"].get<int>();
    return std::pair(x, y);
}

void Self::setPositionTopLeft(int x, int y) {
    assert(isMainThread());
    nlohmann::json json;
    json["x"] = x;
    json["y"] = y;
    bridge_.call(helper_.setPosition(), json.dump());
}

std::pair<int, int> Self::getSize() const {
    assert(isMainThread());
    auto response = bridge_.call(helper_.getSize());
    auto json = nlohmann::json::parse(response);
    auto width = json["width"].get<int>();
    auto height = json["height"].get<int>();
    return std::make_pair(width, height);
}

void Self::setSize(int width, int height) {
    assert(isMainThread());
    auto [currentWidth, currentHeight] = getSize();
    auto [anchorX, anchorY] = getAnchor();
    auto [x, y] = getPositionTopLeft();
    setPositionTopLeft(
        x - static_cast<int>((width - currentWidth) * anchorX),
        y - static_cast<int>((height - currentHeight) * anchorY));

    nlohmann::json json;
    json["width"] = width;
    json["height"] = height;
    bridge_.call(helper_.setSize(), json.dump());
}

bool Self::isVisible() const {
    assert(isMainThread());
    auto response = bridge_.call(helper_.isVisible());
    return core::toBool(response);
}

void Self::setVisible(bool visible) {
    assert(isMainThread());
    bridge_.call(helper_.setVisible(), core::toString(visible));
}
} // namespace ads
} // namespace ee
