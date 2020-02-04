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
constexpr auto k__crashlytics_cause_crash          = "__crashlytics_cause_crash";
constexpr auto k__crashlytics_cause_exception      = "__crashlytics_cause_exception";
constexpr auto k__crashlytics_set_log_level        = "__crashlytics_set_log_level";
constexpr auto k__crashlytics_log                  = "__crashlytics_log";
constexpr auto k__crashlytics_set_string           = "__crashlytics_set_string";
constexpr auto k__crashlytics_set_bool             = "__crashlytics_set_bool";
constexpr auto k__crashlytics_set_int              = "__crashlytics_set_int";
constexpr auto k__crashlytics_set_user_identifier  = "__crashlytics_set_user_identifier";
constexpr auto k__crashlytics_set_user_name        = "__crashlytics_set_user_name";
constexpr auto k__crashlytics_set_user_email       = "__crashlytics_set_user_email";
constexpr auto k__crashlytics_track_level_start    = "__crashlytics_track_level_start";
constexpr auto k__crashlytics_track_level_end      = "__crashlytics_track_level_end";
constexpr auto k__crashlytics_track_purchase       = "__crashlytics_track_purchase";
constexpr auto k__crashlytics_track_custom_event   = "__crashlytics_track_custom_event";
constexpr auto k__crashlytics_track_invite         = "__crashlytics_track_invite";
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

void Crashlytics::setString(const std::string& key,
                            const std::string& value) const {
    nlohmann::json json;
    json["key"] = key;
    json["value"] = value;
    bridge_.call(k__crashlytics_set_string, json.dump());
}

void Crashlytics::setBool(const std::string& key, bool value) const {
    nlohmann::json json;
    json["key"] = key;
    json["value"] = value;
    bridge_.call(k__crashlytics_set_bool, json.dump());
}

void Crashlytics::setInt(const std::string& key, int value) const {
    nlohmann::json json;
    json["key"] = key;
    json["value"] = value;
    bridge_.call(k__crashlytics_set_int, json.dump());
}

void Crashlytics::setUserIdentifier(const std::string& identifier) const {
    bridge_.call(k__crashlytics_set_user_identifier, identifier);
}

void Crashlytics::setUserName(const std::string& name) const {
    bridge_.call(k__crashlytics_set_user_name, name);
}

void Crashlytics::setUserEmail(const std::string& email) const {
    bridge_.call(k__crashlytics_set_user_email, email);
}

void Crashlytics::trackLevelStart(
    const std::string& name,
    const std::unordered_map<std::string, std::string>& attrs) {
    nlohmann::json json;
    json["name"] = name;
    json["attrs"] = attrs;
    bridge_.call(k__crashlytics_track_level_start, json.dump());
}

void Crashlytics::trackLevelEnd(
    const std::string& name, int score, bool success,
    const std::unordered_map<std::string, std::string>& attrs) {
    nlohmann::json json;
    json["name"] = name;
    json["score"] = score;
    json["success"] = success;
    json["attrs"] = attrs;
    bridge_.call(k__crashlytics_track_level_end, json.dump());
}

void Crashlytics::trackPurchase(
    float price, const std::string& currency, bool success,
    const std::string& itemName, const std::string& itemType,
    const std::string& itemId,
    const std::unordered_map<std::string, std::string>& attrs) {
    nlohmann::json json;
    json["price"] = price;
    json["currency"] = currency;
    json["success"] = success;
    json["item_name"] = itemName;
    json["item_type"] = itemType;
    json["item_id"] = itemId;
    json["attrs"] = attrs;
    bridge_.call(k__crashlytics_track_purchase, json.dump());
}

void Crashlytics::trackCustomEvent(
    const std::string& name,
    const std::unordered_map<std::string, std::string>& attrs) {
    nlohmann::json json;
    json["name"] = name;
    json["attrs"] = attrs;
    bridge_.call(k__crashlytics_track_custom_event, json.dump());
}

void Crashlytics::trackInvite(
    const std::string& method,
    const std::unordered_map<std::string, std::string>& attrs) {
    nlohmann::json json;
    json["method"] = method;
    json["attrs"] = attrs;
    bridge_.call(k__crashlytics_track_invite, json.dump());
}
} // namespace crashlytics
} // namespace ee
