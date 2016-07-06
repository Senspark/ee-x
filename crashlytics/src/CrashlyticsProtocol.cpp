//
//  CrashlyticsProtocol.cpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#include "CrashlyticsProtocol.hpp"

#include <PluginProtocol.hpp>
#include <LogLevel.hpp>
#include <json.hpp>

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
#if defined(EE_PLATFORM_IOS) || defined(EE_PLATFORM_MAC)
    json["priorityDescription"] = level.desc;
#elif defined(EE_PLATFORM_ANDROID)
    json["priority"] = level.priority;
#endif
    json["tag"] = tag;
    json["message"] = message;
    protocol_->callNative("log", json.dump());
}
} // namespace crashlytics
} // namespace ee