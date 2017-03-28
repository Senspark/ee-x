//
//  NotificationProtocol.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/Notification.hpp"
#include "ee/internal/MessageBridge.hpp"
#include "ee/libs/nlohmann/json.hpp"

namespace ee {
namespace core {
void Notification::schedule(const std::string& title, const std::string& body,
                            int delay, const std::string& tag) {
    schedule(title, body, delay, 0, tag);
}

void Notification::schedule(const std::string& title, const std::string& body,
                            int delay, int interval, const std::string& tag) {
    nlohmann::json json;
    json["title"] = title;
    json["body"] = body;
    json["delay"] = delay;
    json["interval"] = interval;
    json["tag"] = tag;
    MessageBridge::getInstance().call("__notification_schedule", json.dump());
}

void Notification::unscheduleAll() {
    MessageBridge::getInstance().call("__notification_unschedule_all");
}

void Notification::unschedule(const std::string& tag) {
    MessageBridge::getInstance().call("__notification_unschedule", tag);
}
} // namespace core
} // namespace ee
