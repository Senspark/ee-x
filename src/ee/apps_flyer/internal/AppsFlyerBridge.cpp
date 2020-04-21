//
//  AppsFlyerBridge.cpp
//  ee_x_mobile_apps_flyer
//
//  Created by eps on 6/26/18.
//

#include "ee/apps_flyer/internal/AppsFlyerBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/core/PluginManager.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

namespace ee {
namespace core {
template <>
std::unique_ptr<IAppsFlyer> PluginManager::createPlugin() {
    return std::make_unique<apps_flyer::Bridge>();
}
} // namespace core

namespace apps_flyer {
namespace {
// clang-format off
const std::string kPrefix = "AppsFlyer";

const auto kInitialize      = kPrefix + "Initialize";
const auto kStartTracking   = kPrefix + "StartTracking";
const auto kGetDeviceId     = kPrefix + "GetDeviceId";
const auto kSetDebugEnabled = kPrefix + "SetDebugEnabled";
const auto kSetStopTracking = kPrefix + "SetStopTracking";
const auto kTrackEvent      = kPrefix + "TrackEvent";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge()
    : bridge_(MessageBridge::getInstance()) {
    //
}

Self::~Bridge() {
    //
}

void Self::initialize(const std::string& devKey, const std::string& appId) {
    runOnUiThread([this, devKey, appId] {
#ifdef EE_X_ANDROID
        bridge_.call(kInitialize, devKey);
#else  // EE_X_ANDROID
        nlohmann::json json;
        json["devKey"] = devKey;
        json["appId"] = appId;
        bridge_.call(kInitialize, json.dump());
#endif // EE_X_ANDROID
    });
}

void Self::startTracking() {
    runOnUiThread([this] { //
        bridge_.call(kStartTracking);
    });
}

std::string Self::getDeviceId() const {
    return bridge_.call(kGetDeviceId);
}

void Self::setDebugEnabled(bool enabled) {
    runOnUiThread([this, enabled] {
        bridge_.call(kSetDebugEnabled, core::toString(enabled));
    });
}

void Self::setStopTracking(bool enabled) {
    runOnUiThread([this, enabled] {
        bridge_.call(kSetStopTracking, core::toString(enabled));
    });
}

void Self::trackEvent(const std::string& name,
                      const std::map<std::string, std::string>& values) {
    runOnUiThread([this, name, values] {
        nlohmann::json json;
        json["name"] = name;
        json["values"] = values;
        bridge_.call(kTrackEvent, json.dump());
    });
}
} // namespace apps_flyer
} // namespace ee
