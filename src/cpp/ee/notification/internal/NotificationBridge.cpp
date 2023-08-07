//
//  NotificationProtocol.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/notification/internal/NotificationBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Platform.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/notification/NotificationBuilder.hpp"

namespace ee {
namespace notification {
namespace {
const std::string kPrefix = "NotificationBridge";
const auto kSchedule = kPrefix + "Schedule";
const auto kUnschedule = kPrefix + "Unschedule";
const auto kClearAll = kPrefix + "ClearAll";
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer) {}

Self::~Bridge() = default;

void Self::destroy() {
    destroyer_();
}

void Self::schedule(const NotificationBuilder& builder) {
    nlohmann::json json;
    json["ticker"] = builder.ticker_;
    json["title"] = builder.title_.empty() ? Platform::getApplicationName()
                                           : builder.title_;
    json["body"] = builder.body_;
    json["delay"] = builder.delay_;
    json["interval"] = builder.interval_;
    json["tag"] = builder.tag_;
    json["style"] = builder.style_;
    bridge_.call(kSchedule, json.dump());
}

void Self::schedule(const std::string& msg, int tag, int style, int delay, int interval) {
    auto builder = NotificationBuilder();
    builder.setBody(msg);
    builder.setDelay(delay);
    builder.setInterval(interval);
    builder.setTag(tag);
    builder.setStyle(style);
    schedule(builder);
}

void Self::unschedule(int tag) {
    nlohmann::json json;
    json["tag"] = tag;
    bridge_.call(kUnschedule, json.dump());
}

void Self::clearAll() {
    bridge_.call(kClearAll);
}
} // namespace notification
} // namespace ee
