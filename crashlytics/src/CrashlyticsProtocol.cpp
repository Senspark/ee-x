//
//  CrashlyticsProtocol.cpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#include "CrashlyticsProtocol.hpp"

#include <json.hpp>

namespace ee {
namespace crashlytics {
LogLevel::LogLevel(int priority, const std::string& desc)
    : priority_{priority}
    , desc_{desc} {}

const LogLevel LogLevel::Verbose(2, "[VERBOSE]");
const LogLevel LogLevel::Debug(3, "[DEBUG]");
const LogLevel LogLevel::Info(4, "[INFO]");
const LogLevel LogLevel::Warn(5, "[WARN]");
const LogLevel LogLevel::Error(6, "[ERROR]");
const LogLevel LogLevel::Assert(7, "[ASSERT]");

CrashlyticsProtocol::CrashlyticsProtocol()
    : PluginProtocol("CrashlyticsProtocol") {}

void CrashlyticsProtocol::causeCrash() const { callNative("causeCrash"); }

void CrashlyticsProtocol::causeException() const {
    callNative("causeException");
}

void CrashlyticsProtocol::log(const LogLevel& level, const std::string& tag,
                              const std::string& message) const {
    nlohmann::json json;
#if defined(EE_PLATFORM_IOS) || defined(EE_PLATFORM_MAC)
    json["priorityDescription"] = level.desc_;
#elif defined(EE_PLATFORM_ANDROID)
    json["priority"] = level.priority_;
#endif
    json["tag"] = tag;
    json["message"] = message;
    callNative("log", json.dump());
}
} // namespace crashlytics
} // namespace ee