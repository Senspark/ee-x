//
//  CrashlyticsProtocol.cpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/crashlytics/CrashlyticsBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/core/LogLevel.hpp>
#include <ee/core/internal/MessageBridge.hpp>

namespace ee {
namespace crashlytics {
using Self = Bridge;

namespace {
// clang-format off
constexpr auto k__crashlytics_cause_crash     = "__crashlytics_cause_crash";
constexpr auto k__crashlytics_cause_exception = "__crashlytics_cause_exception";
constexpr auto k__crashlytics_set_log_level   = "__crashlytics_set_log_level";
constexpr auto k__crashlytics_log             = "__crashlytics_log";
// clang-format on
} // namespace

Self::Bridge()
    : bridge_(MessageBridge::getInstance()) {}

void Crashlytics::causeCrash() const {
    bridge_.call(k__crashlytics_cause_crash);
}

void Crashlytics::causeException() const {
    bridge_.call(k__crashlytics_cause_exception);
}

void Crashlytics::setLogLevel(const core::LogLevel& level) const {
    nlohmann::json json;
    json["priority"] = level.priority;
    bridge_.call(k__crashlytics_set_log_level, json.dump());
}

void Crashlytics::log(const core::LogLevel& level, const std::string& tag,
                      const std::string& message) const {
    nlohmann::json json;
    json["priorityDescription"] = level.desc;
    json["priority"] = level.priority;
    json["tag"] = tag;
    json["message"] = message;
    bridge_.call(k__crashlytics_log, json.dump());
}
} // namespace crashlytics
} // namespace ee
