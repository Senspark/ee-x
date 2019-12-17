//
//  NotificationProtocol.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/notification/NotificationBridge.hpp"

#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include "ee/notification/NotificationBuilder.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace notification {
using Self = Notification;

Self::Notification()
    : bridge_(MessageBridge::getInstance()) {}

void Self::schedule(const NotificationBuilder& builder) {
    nlohmann::json json;
    json["title"] =
        builder.title_.empty() ? core::getApplicationName() : builder.title_;
    json["body"] = builder.body_;
    json["delay"] = builder.delay_;
    json["interval"] = builder.interval_;
    json["tag"] = builder.tag_;
    bridge_.call("__notification_schedule", json.dump());
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
    bridge_.call("__notification_unschedule", json.dump());
}

void Self::unscheduleAll() {
    bridge_.call("__notification_unschedule_all");
}

void Self::clearAll() {
    bridge_.call("__notification_clear_all");
}
} // namespace notification
} // namespace ee
