//
//  CrashlyticsProtocol.cpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/CrashlyticsProtocol.hpp"

#include <ee/PluginProtocol.hpp>
#include <ee/LogLevel.hpp>
#include <nlohmann/json.hpp>

namespace ee {
namespace crashlytics {
CrashlyticsProtocol::CrashlyticsProtocol()
    : protocol_{new core::PluginProtocol{"CrashlyticsProtocol"}} {}

CrashlyticsProtocol::~CrashlyticsProtocol() = default;

void CrashlyticsProtocol::causeCrash() const {
    protocol_->callNative("causeCrash");
}

void CrashlyticsProtocol::causeException() const {
    protocol_->callNative("causeException");
}

void CrashlyticsProtocol::log(const core::LogLevel& level,
                              const std::string& tag,
                              const std::string& message) const {
    nlohmann::json json;
    json["priorityDescription"] = level.desc;
    json["priority"] = level.priority;
    json["tag"] = tag;
    json["message"] = message;
    protocol_->callNative("log", json.dump());
}

void CrashlyticsProtocol::setString(const std::string& key,
                                    const std::string& value) const {
    nlohmann::json json;
    json["key"] = key;
    json["value"] = value;
    protocol_->callNative("setString", json.dump());
}

void CrashlyticsProtocol::setBool(const std::string& key, bool value) const {
    nlohmann::json json;
    json["key"] = key;
    json["value"] = value;
    protocol_->callNative("setBool", json.dump());
}

void CrashlyticsProtocol::setInt(const std::string& key, int value) const {
    nlohmann::json json;
    json["key"] = key;
    json["value"] = value;
    protocol_->callNative("setInt", json.dump());
}

void CrashlyticsProtocol::setUserIdentifier(
    const std::string& identifier) const {
    nlohmann::json json;
    json["identifier"] = identifier;
    protocol_->callNative("setUserIdentifier", json.dump());
}

void CrashlyticsProtocol::setUserName(const std::string& name) const {
    nlohmann::json json;
    json["name"] = name;
    protocol_->callNative("setUserName", json.dump());
}

void CrashlyticsProtocol::setUserEmail(const std::string& email) const {
    nlohmann::json json;
    json["email"] = email;
    protocol_->callNative("setUserEmail", json.dump());
}
} // namespace crashlytics
} // namespace ee