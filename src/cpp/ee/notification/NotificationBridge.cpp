//
//  NotificationProtocol.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/notification/NotificationBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Platform.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/notification/NotificationBuilder.hpp"

namespace ee {
namespace notification {
namespace {
const std::string kPrefix = "NotificationBridge";
const auto kSchedule = kPrefix + "Schedule";
const auto kUnschedule = kPrefix + "Unschedule";
const auto kUnscheduleAll = kPrefix + "UnscheduleAll";
const auto kClearAll = kPrefix + "ClearAll";
} // namespace

using Self = Bridge;

Self::Bridge()
    : bridge_(PluginManager::getBridge()) {
    PluginManager::addPlugin(Plugin::Notification);
}

void Self::destroy() {
    PluginManager::removePlugin(Plugin::Notification);
}

void Self::schedule(const NotificationBuilder& builder) {
    nlohmann::json json;
    json["ticker"] = builder.ticker_;
    json["title"] =
        builder.title_.empty() ? getApplicationName() : builder.title_;
    json["body"] = builder.body_;
    json["delay"] = builder.delay_;
    json["interval"] = builder.interval_;
    json["tag"] = builder.tag_;
    bridge_.call(kSchedule, json.dump());
}

void Self::schedule(const std::string& msg, int tag, int delay, int interval) {
    auto builder = NotificationBuilder();
    builder.setBody(msg);
    builder.setDelay(delay);
    builder.setInterval(interval);
    builder.setTag(tag);
    schedule(builder);
}

void Self::unschedule(int tag) {
    nlohmann::json json;
    json["tag"] = tag;
    bridge_.call(kUnschedule, json.dump());
}

void Self::unscheduleAll() {
    bridge_.call(kUnscheduleAll);
}

void Self::clearAll() {
    bridge_.call(kClearAll);
}
} // namespace notification
} // namespace ee
