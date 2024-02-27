//
//  NotificationBuilder.cpp
//  ee_x
//
//  Created by Zinge on 4/3/17.
//
//

#include "ee/notification/NotificationBuilder.hpp"

namespace ee {
namespace notification {
NotificationBuilder::NotificationBuilder()
    : ticker_("")
    , title_("")
    , body_("")
    , delay_(0)
    , interval_(0)
    , tag_(0) {}

NotificationBuilder& NotificationBuilder::setTicker(const std::string& ticker) {
    ticker_ = ticker;
    return *this;
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

NotificationBuilder& NotificationBuilder::setStyle(int style) {
    style_ = style;
    return *this;
}
} // namespace notification
} // namespace ee
