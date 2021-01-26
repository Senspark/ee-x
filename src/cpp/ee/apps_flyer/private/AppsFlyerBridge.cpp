//
//  AppsFlyerBridge.cpp
//  ee_x_mobile_apps_flyer
//
//  Created by eps on 6/26/18.
//

#include "ee/apps_flyer/private/AppsFlyerBridge.hpp"

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace apps_flyer {
namespace {
// clang-format off
const std::string kPrefix   = "AppsFlyerBridge";
const auto kInitialize      = kPrefix + "Initialize";
const auto kStartTracking   = kPrefix + "StartTracking";
const auto kGetDeviceId     = kPrefix + "GetDeviceId";
const auto kSetDebugEnabled = kPrefix + "SetDebugEnabled";
const auto kSetStopTracking = kPrefix + "SetStopTracking";
const auto kTrackEvent      = kPrefix + "TrackEvent";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer) {}

Self::~Bridge() = default;

void Self::destroy() {
    destroyer_();
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
} // namespace apps_flyer
} // namespace ee
