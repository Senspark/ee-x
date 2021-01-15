//
//  VideoPlayer.cpp
//  ee_x_mobile_core
//
//  Created by eps on 1/30/18.
//

#include "ee/core/VideoPlayer.hpp"

#include <ee/nlohmann/json.hpp>

#include "ee/core/IMessageBridge.hpp"
#include "ee/core/Utils.hpp"

namespace ee {
namespace core {
using Self = VideoPlayer;

namespace {
std::string k__loadFile(const std::string& tag) {
    return format("VideoPlayer_loadFile_%s", tag.c_str());
}

std::string k__setPosition(const std::string& tag) {
    return format("VideoPlayer_setPosition_%s", tag.c_str());
}

std::string k__setSize(const std::string& tag) {
    return format("VideoPlayer_setSize_%s", tag.c_str());
}

std::string k__play(const std::string& tag) {
    return format("VideoPlayer_play_%s", tag.c_str());
}

std::string k__pause(const std::string& tag) {
    return format("VideoPlayer_pause_%s", tag.c_str());
}

std::string k__resume(const std::string& tag) {
    return format("VideoPlayer_resume_%s", tag.c_str());
}

std::string k__stop(const std::string& tag) {
    return format("VideoPlayer_stop_%s", tag.c_str());
}

std::string k__setVisible(const std::string& tag) {
    return format("VideoPlayer_setVisible_%s", tag.c_str());
}

std::string k__setKeepAspectRatioEnabled(const std::string& tag) {
    return format("VideoPlayer_setKeepAspectRatioEnabled_%s", tag.c_str());
}

std::string k__setFullScreenEnabled(const std::string& tag) {
    return format("VideoPlayer_setFullScreenEnabled_%s", tag.c_str());
}
} // namespace

Self::VideoPlayer(IMessageBridge& bridge, const std::string& tag)
    : bridge_(bridge)
    , tag_(tag) {
    //
}

Self::~VideoPlayer() {
    //
}

void Self::loadFile(const std::string& path) {
    bridge_.call(k__loadFile(tag_), path);
}

void Self::setPosition(int x, int y) {
    nlohmann::json json;
    json["x"] = x;
    json["y"] = y;
    bridge_.call(k__setPosition(tag_), json.dump());
}

void Self::setSize(int width, int height) {
    nlohmann::json json;
    json["width"] = width;
    json["height"] = height;
    bridge_.call(k__setSize(tag_), json.dump());
}

void Self::play() {
    bridge_.call(k__play(tag_));
}

void Self::pause() {
    bridge_.call(k__pause(tag_));
}

void Self::resume() {
    bridge_.call(k__resume(tag_));
}

void Self::stop() {
    bridge_.call(k__stop(tag_));
}

bool Self::isVisible() const {
    return false;
}

void Self::setVisible(bool visible) {
    bridge_.call(k__setVisible(tag_), toString(visible));
}

bool Self::isKeepAspectRatioEnabled() const {
    return false;
}

void Self::setKeepAspectRatioEnabled(bool enabled) {
    bridge_.call(k__setKeepAspectRatioEnabled(tag_), toString(enabled));
}

bool Self::isFullScreenEnabled() const {
    return false;
}

void Self::setFullScreenEnabled(bool enabled) {
    bridge_.call(k__setFullScreenEnabled(tag_), toString(enabled));
}
} // namespace core
} // namespace ee
