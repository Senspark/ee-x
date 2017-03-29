//
//  CrashlyticsProtocol.cpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/Crashlytics.hpp"

#include <ee/LogLevel.hpp>
#include <ee/internal/MessageBridge.hpp>
#include <ee/internal/PluginManager.hpp>
#include <ee/libs/nlohmann/json.hpp>

namespace ee {
namespace crashlytics {
Crashlytics::Crashlytics() {
    core::PluginManager::getInstance().addPlugin("Crashlytics");
}
    
Crashlytics::~Crashlytics() {
    core::PluginManager::getInstance().removePlugin("Crashlytics");
}

void Crashlytics::causeCrash() const {
    core::MessageBridge::getInstance().call("__crashlytics_cause_crash");
}

void Crashlytics::causeException() const {
    core::MessageBridge::getInstance().call("__crashlytics_cause_exception");
}

void Crashlytics::log(const core::LogLevel& level, const std::string& tag,
                      const std::string& message) const {
    nlohmann::json json;
    json["priorityDescription"] = level.desc;
    json["priority"] = level.priority;
    json["tag"] = tag;
    json["message"] = message;
    core::MessageBridge::getInstance().call("__crashlytics_log", json.dump());
}

void Crashlytics::setString(const std::string& key,
                            const std::string& value) const {
    nlohmann::json json;
    json["key"] = key;
    json["value"] = value;
    core::MessageBridge::getInstance().call("__crashlytics_set_string",
                                            json.dump());
}

void Crashlytics::setBool(const std::string& key, bool value) const {
    nlohmann::json json;
    json["key"] = key;
    json["value"] = value;
    core::MessageBridge::getInstance().call("__crashlytics_set_bool",
                                            json.dump());
}

void Crashlytics::setInt(const std::string& key, int value) const {
    nlohmann::json json;
    json["key"] = key;
    json["value"] = value;
    core::MessageBridge::getInstance().call("__crashlytics_set_int",
                                            json.dump());
}

void Crashlytics::setUserIdentifier(const std::string& identifier) const {
    core::MessageBridge::getInstance().call("__crashlytics_set_user_identifier",
                                            identifier);
}

void Crashlytics::setUserName(const std::string& name) const {
    core::MessageBridge::getInstance().call("__crashlytics_set_user_user_name",
                                            name);
}

void Crashlytics::setUserEmail(const std::string& email) const {
    core::MessageBridge::getInstance().call("__crashlytics_set_user_email",
                                            email);
}
} // namespace crashlytics
} // namespace ee
