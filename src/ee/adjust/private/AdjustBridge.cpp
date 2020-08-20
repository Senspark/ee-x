//
//  AdjustBridge.cpp
//  Adjust
//
//  Created by eps on 8/19/20.
//

#include "ee/adjust/private/AdjustBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/adjust/AdjustConfig.hpp"

namespace ee {
namespace core {
template <>
std::shared_ptr<IAdjust>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    if (not addPlugin(Plugin::Adjust)) {
        return nullptr;
    }
    return std::make_shared<adjust::Bridge>(bridge);
}
} // namespace core

namespace adjust {
namespace {
const std::string kPrefix = "AdjustBridge";
const auto kInitialize = kPrefix + "Initialize";
const auto kSetEnabled = kPrefix + "SetEnabled";
const auto kGetAdvertisingIdentifier = kPrefix + "GetAdvertisingIdentifier";
const auto kGetDeviceIdentifier = kPrefix + "GetDeviceIdentifier";
const auto kSetPushToken = kPrefix + "SetPushToken";
const auto kTrackEvent = kPrefix + "TrackEvent";
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge) {}

Self::~Bridge() = default;

void Self::destroy() {
    PluginManager::removePlugin(Plugin::Adjust);
}

void Self::initialize(const AdjustConfig& config) {
    nlohmann::json json;
    json["token"] = config.token_;
    json["environment"] = config.environment_;
    json["logLevel"] = config.logLevel_;
    bridge_.call(kInitialize, json.dump());
}

void Self::setEnabled(bool enabled) {
    bridge_.call(kSetEnabled, core::toString(enabled));
}

Task<std::string> Self::getAdvertisingIdentifier() const {
    auto response = co_await bridge_.callAsync(kGetAdvertisingIdentifier);
    co_return response;
}

std::string Self::getDeviceIdentifier() const {
    auto response = bridge_.call(kGetDeviceIdentifier);
    return response;
}

void Self::setPushToken(const std::string& token) {
    bridge_.call(kSetPushToken, token);
}

void Self::trackEvent(const std::string& token) {
    bridge_.call(kTrackEvent, token);
}
} // namespace adjust
} // namespace ee
