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

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace google {
using Self = AnalyticsTracker;

namespace {
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

void Self::setAllowIDFACollection(bool enabled) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__setAllowIDFACollection(trackingId_),
                core::toString(enabled));
}

void Self::send(const std::unordered_map<std::string, std::string>& dict) {
    nlohmann::json json = dict;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__send(trackingId_), json.dump());
}
} // namespace google
} // namespace ee
