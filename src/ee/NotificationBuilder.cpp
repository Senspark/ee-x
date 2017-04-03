//
//  NotificationBuilder.cpp
//  ee_x
//
//  Created by Zinge on 4/3/17.
//
//

#include "NotificationBuilder.hpp"

namespace ee {
NotificationBuilder::NotificationBuilder()
    : title_("")
    , body_("")
    , delay_(0)
    , interval_(0)
    , tag_(0) {
}

NotificationBuilder& NotificationBuilder::setTitle(const std::string& title) {
    title_ = title;
    return *this;
}

NotificationBuilder& NotificationBuilder::setBody(const std::string& body) {
    body_ = body;
    return *this;
}

NotificationBuilder& NotificationBuilder::setDelay(int delay) {
    delay_ = delay;
    return *this;
}

NotificationBuilder& NotificationBuilder::setInterval(int interval) {
    interval_ = interval;
    return *this;
}

NotificationBuilder& NotificationBuilder::setTag(int tag) {
    tag_ = tag;
    return *this;
}
} // namespace ee
