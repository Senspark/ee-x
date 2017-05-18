//
//  NotificationAgent.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "NotificationAgent.hpp"

#include <ee/Notification.hpp>

namespace tag {
constexpr int notification_0 = 0;
constexpr int notification_1 = 1;
constexpr int notification_2 = 2;
} // namespace tag

NotificationAgent* NotificationAgent::getInstance() {
    static NotificationAgent sharedInstance;
    return &sharedInstance;
}

void NotificationAgent::initialize() {
    protocol_ = std::make_unique<ee::Notification>();
}

void NotificationAgent::scheduleAll() {
    // Immediately.
    protocol_->schedule(ee::NotificationBuilder()
                            .setTitle("Title 1")
                            .setBody("Body 1")
                            .setDelay(0)
                            .setTag(tag::notification_0));

    // Delayed 5 minutes.
    protocol_->schedule(ee::NotificationBuilder()
                            .setTitle("Title 2")
                            .setBody("Body 2")
                            .setDelay(300)
                            .setTag(tag::notification_1));

    // Delayed 10 minutes and repeat every 5 minutes.
    protocol_->schedule(ee::NotificationBuilder()
                            .setTitle("Title 3")
                            .setBody("Body 3")
                            .setDelay(600)
                            .setInterval(300)
                            .setTag(tag::notification_2));
}

void NotificationAgent::unscheduleAll() {
    protocol_->unschedule(tag::notification_0);
    protocol_->unschedule(tag::notification_1);
    protocol_->unschedule(tag::notification_2);

    // Also clear all notifications.
    protocol_->clearAll();
}
