//
//  NotificationAgent.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "NotificationTester.hpp"

#include <ee/Cpp.hpp>

namespace eetest {
namespace notification {
namespace tag {
constexpr int notification_0 = 0;
constexpr int notification_1 = 1;
constexpr int notification_2 = 2;
} // namespace tag

using Self = Tester;

void Self::initialize() {
    plugin_ = std::make_shared<ee::Notification>();
}

void Self::destroy() {
    plugin_->destroy();
}

void Self::start() {
    // Immediately.
    plugin_->schedule(ee::NotificationBuilder()
                          .setTitle("Title 1")
                          .setBody("Body 1")
                          .setDelay(0)
                          .setTag(tag::notification_0));

    // Delayed 5 minutes.
    plugin_->schedule(ee::NotificationBuilder()
                          .setTitle("Title 2")
                          .setBody("Body 2")
                          .setDelay(300)
                          .setTag(tag::notification_1));

    // Delayed 10 minutes and repeat every 5 minutes.
    plugin_->schedule(ee::NotificationBuilder()
                          .setTitle("Title 3")
                          .setBody("Body 3")
                          .setDelay(600)
                          .setInterval(300)
                          .setTag(tag::notification_2));
}

void Self::stop() {
    plugin_->unschedule(tag::notification_0);
    plugin_->unschedule(tag::notification_1);
    plugin_->unschedule(tag::notification_2);

    // Also clear all notifications.
    plugin_->clearAll();
}
} // namespace notification
} // namespace eetest
