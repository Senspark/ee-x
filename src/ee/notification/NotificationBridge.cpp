//
//  NotificationProtocol.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/notification/NotificationBridge.hpp"
#include "ee/notification/NotificationBuilder.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace notification {
void Notification::schedule(const NotificationBuilder& builder) {
    nlohmann::json json;
    json["title"] = builder.title_;
    json["body"] = builder.body_;
    json["delay"] = builder.delay_;
    json["interval"] = builder.interval_;
    json["tag"] = builder.tag_;
    core::MessageBridge::getInstance().call("__notification_schedule",
                                            json.dump());
}

void Notification::unschedule(int tag) {
    nlohmann::json json;
    json["tag"] = tag;
    core::MessageBridge::getInstance().call("__notification_unschedule",
                                            json.dump());
}

void Notification::unscheduleAll() {
    core::MessageBridge::getInstance().call("__notification_unschedule_all");
}

void Notification::clearAll() {
    core::MessageBridge::getInstance().call("__notification_clear_all");
}
} // namespace notification
} // namespace ee
