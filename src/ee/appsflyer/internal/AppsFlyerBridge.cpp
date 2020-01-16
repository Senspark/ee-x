//
//  AppsFlyerBridge.cpp
//  ee_x_mobile_apps_flyer
//
//  Created by eps on 6/26/18.
//

#include "ee/appsflyer/internal/AppsFlyerBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/core/MessageBridge.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Utils.hpp>

namespace ee {
namespace core {
template <>
std::unique_ptr<IAppsFlyer> PluginManager::createPlugin() {
    return std::make_unique<appsflyer::Bridge>();
}
} // namespace core

namespace appsflyer {
namespace {
// clang-format off
constexpr auto kInitialize      = "AppsFlyerInitialize";
constexpr auto kStartTracking   = "AppsFlyerStartTracking";
constexpr auto kGetDeviceId     = "AppsFlyerGetDeviceId";
constexpr auto kSetDebugEnabled = "AppsFlyerSetDebugEnabled";
constexpr auto kSetStopTracking = "AppsFlyerSetStopTracking";
constexpr auto kTrackEvent      = "AppsFlyerTrackEvent";
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
#ifdef EE_X_ANDROID
    bridge_.call(kInitialize, devKey);
#else  // EE_X_ANDROID
    nlohmann::json json;
    json["devKey"] = devKey;
    json["appId"] = appId;
    bridge_.call(kInitialize, json.dump());
#endif // EE_X_ANDROID
}

void Self::startTracking() {
    bridge_.call(kStartTracking);
}

std::string Self::getDeviceId() const {
    return bridge_.call(kGetDeviceId);
}

void Self::setDebugEnabled(bool enabled) {
    bridge_.call(kSetDebugEnabled, core::toString(enabled));
}

void Self::setStopTracking(bool enabled) {
    bridge_.call(kSetStopTracking, core::toString(enabled));
}

void Self::trackEvent(const std::string& name,
                      const std::map<std::string, std::string>& values) {
    nlohmann::json json;
    json["name"] = name;
    json["values"] = values;
    bridge_.call(kTrackEvent, json.dump());
}
} // namespace appsflyer
} // namespace ee
