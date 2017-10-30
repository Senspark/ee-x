//
//  GoogleAnalyticsTracker.cpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#include "ee/google/GoogleAnalyticsTracker.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"
#include "ee/google/GoogleAnalyticsBridge.hpp"
#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace google {
using Self = AnalyticsTracker;

namespace {
// clang-format off
constexpr auto k__key   = "key";
constexpr auto k__value = "value";
// clang-format on
} // namespace

namespace {
std::string k__setParameter(const std::string& id) {
    return "GoogleAnalytics_setParameter_" + id;
}

std::string k__setAllowIDFACollection(const std::string& id) {
    return "GoogleAnalytics_setAllowIDFACollection_" + id;
}

std::string k__send(const std::string& id) {
    return "GoogleAnalytics_send_" + id;
}
} // namespace

Self::AnalyticsTracker(Analytics* plugin, const std::string& trackingId) {
    plugin_ = plugin;
    trackingId_ = trackingId;
}

Self::~AnalyticsTracker() {
    plugin_->destroyTracker(trackingId_);
}

void Self::setParameter(const std::string& key, const std::string& value) {
    nlohmann::json json;
    json[k__key] = key;
    json[k__value] = value;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__setParameter(trackingId_), json.dump());
}

void Self::setAllowIDFACollection(bool enabled) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__setAllowIDFACollection(trackingId_),
                core::toString(enabled));
}

void Self::setScreenName(const std::string& screenName) {
    setParameter(make_parameter(parameter::screen_name), screenName);
}

void Self::send(const TrackingDict& dict) {
    nlohmann::json json = dict;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__send(trackingId_), json.dump());
}
} // namespace google
} // namespace ee
