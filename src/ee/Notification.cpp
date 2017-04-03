//
//  NotificationProtocol.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/Notification.hpp"
#include "ee/NotificationBuilder.hpp"
#include "ee/internal/MessageBridge.hpp"
#include "ee/libs/nlohmann/json.hpp"

namespace ee {
void Notification::schedule(const NotificationBuilder& builder) {
    nlohmann::json json;
    json["title"] = builder.title_;
    json["body"] = builder.body_;
    json["delay"] = builder.delay_;
    json["interval"] = builder.interval_;
    json["tag"] = builder.tag_;
    MessageBridge::getInstance().call("__notification_schedule", json.dump());
}

void Notification::unschedule(int tag) {
    nlohmann::json json;
    json["tag"] = tag;
    MessageBridge::getInstance().call("__notification_unschedule", json.dump());
}

void Notification::unscheduleAll() {
    MessageBridge::getInstance().call("__notification_unschedule_all");
}
} // namespace ee
