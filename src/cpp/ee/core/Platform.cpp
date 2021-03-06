//
//  PlatformUtils.cpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#include "ee/core/Platform.hpp"

#include <ee/nlohmann/json.hpp>

#include "ee/core/IMessageBridge.hpp"
#include "ee/core/Task.hpp"
#include "ee/core/Utils.hpp"

namespace ee {
namespace core {
namespace {
// clang-format off
const std::string kPrefix = "Platform_";

const auto kIsApplicationInstalled   = kPrefix + "isApplicationInstalled";
const auto kOpenApplication          = kPrefix + "openApplication";

const auto kGetApplicationId         = kPrefix + "getApplicationId";
const auto kGetApplicationName       = kPrefix + "getApplicationName";
const auto kGetVersionName           = kPrefix + "getVersionName";
const auto kGetVersionCode           = kPrefix + "getVersionCode";

const auto kGetApplicationSignatures = kPrefix + "getApplicationSignatures";
const auto kIsInstantApp             = kPrefix + "isInstantApp";
const auto kIsTablet                 = kPrefix + "isTablet";
const auto kGetDensity               = kPrefix + "getDensity";
const auto kGetViewSize              = kPrefix + "getViewSize";
const auto kGetScreenSize            = kPrefix + "getScreenSize";
const auto kGetDeviceId              = kPrefix + "getDeviceId";
const auto kGetSafeInset             = kPrefix + "getSafeInset";

const auto kSendMail                 = kPrefix + "sendMail";
const auto kTestConnection           = kPrefix + "testConnection";
const auto kRequestTrackingAuthorization = kPrefix + "requestTrackingAuthorization";

const auto kShowInstallPrompt        = kPrefix + "showInstallPrompt";
const auto kGetInstallReferrer       = kPrefix + "getInstallReferrer";
// clang-format on
} // namespace

using Self = Platform;

IMessageBridge* Self::bridge_ = nullptr;

void Self::initialize(IMessageBridge& bridge) {
    bridge_ = &bridge;
}

bool Self::isApplicationInstalled(const std::string& applicationId) {
    auto response = bridge_->call(kIsApplicationInstalled, applicationId);
    return toBool(response);
}

bool Self::openApplication(const std::string& applicationId) {
    auto response = bridge_->call(kOpenApplication, applicationId);
    return toBool(response);
}

std::string Self::getApplicationId() {
    return bridge_->call(kGetApplicationId);
}

std::string Self::getApplicationName() {
    return bridge_->call(kGetApplicationName);
}

std::string Self::getVersionName() {
    return bridge_->call(kGetVersionName);
}

std::string Self::getVersionCode() {
    return bridge_->call(kGetVersionCode);
}

std::string Self::getSHA1CertificateFingerprint() {
#ifdef EE_X_ANDROID
    nlohmann::json request;
    request["algorithm"] = "SHA1";
    auto response = bridge_->call(kGetApplicationSignatures, request.dump());
    auto json = nlohmann::json::parse(response);
    auto signatures = json["signatures"];
    return signatures.empty() ? "" : signatures[0];
#else  // EE_X_ANDROID
    return "";
#endif // EE_X_ANDROID
}

bool Self::isInstantApp() {
#ifdef EE_X_ANDROID
    auto response = bridge_->call(kIsInstantApp);
    return toBool(response);
#else  // EE_X_ANDROID
    return false;
#endif // EE_X_ANDROID
}

bool Self::isTablet() {
    auto response = bridge_->call(kIsTablet);
    return toBool(response);
}

float Self::getDensity() {
    auto response = bridge_->call(kGetDensity);
    return std::stof(response);
}

std::pair<int, int> Self::getViewSize() {
    auto response = bridge_->call(kGetViewSize);
    auto json = nlohmann::json::parse(response);
    return std::pair(json["width"], json["height"]);
}

std::pair<int, int> Self::getScreenSize() {
    auto response = bridge_->call(kGetScreenSize);
    auto json = nlohmann::json::parse(response);
    return std::pair(json["width"], json["height"]);
}

Task<std::string> Self::getDeviceId() {
    auto response = co_await bridge_->callAsync(kGetDeviceId);
    co_return response;
}

SafeInset Self::getSafeInset() {
    SafeInset result;
#ifdef EE_X_ANDROID
    auto response = bridge_->call(kGetSafeInset);
    auto json = nlohmann::json::parse(response);

    result.left = json["left"];
    result.right = json["right"];
    result.top = json["top"];
    result.bottom = json["bottom"];
#else  // EE_X_ANDROID
    // TODO.
    result.left = 0;
    result.right = 0;
    result.top = 0;
    result.bottom = 0;
#endif // EE_X_ANDROID
    return result;
}

bool Self::sendMail(const std::string& recipient, const std::string& subject,
                    const std::string& body) {
    nlohmann::json json;
    json["recipient"] = recipient;
    json["subject"] = subject;
    json["body"] = body;
    auto response = bridge_->call(kSendMail, json.dump());
    return toBool(response);
}

Task<bool> Self::testConnection(const std::string& hostName, float timeOut) {
    nlohmann::json json;
    json["host_name"] = hostName;
    json["time_out"] = timeOut;
    auto response = co_await bridge_->callAsync(kTestConnection, json.dump());
    co_return toBool(response);
}

Task<AuthorizationStatus> Self::requestTrackingAuthorization() {
#ifdef EE_X_ANDROID
    co_return AuthorizationStatus::Other;
#else // EE_X_ANDROID
    auto response = co_await bridge_->callAsync(kRequestTrackingAuthorization);
    auto result = std::stoi(response);
    co_return static_cast<AuthorizationStatus>(result);
#endif // EE_X_ANDROID
}

void Self::showInstallPrompt(const std::string& url,
                             const std::string& referrer) {
#ifdef EE_X_ANDROID
    nlohmann::json json;
    json["url"] = url;
    json["referrer"] = referrer;
    bridge_->call(kShowInstallPrompt, json.dump());
#endif // EE_X_ANDROID
}

Task<InstallReferrer> Self::getInstallReferrer() {
    InstallReferrer result;
#ifdef EE_X_ANDROID
    auto response = co_await bridge_->callAsync(kGetInstallReferrer);
    result.raw = response;
#else  // EE_X_ANDROID
    result.raw = "";
#endif // EE_X_ANDROID
    auto params = core::split(result.raw, "&");
    std::map<std::string, std::string> m;
    for (auto&& param : params) {
        auto entries = core::split(param, "=");
        if (entries.size() >= 2) {
            m[entries[0]] = entries[1];
        }
    }
    result.utm_source = m["utm_source"];
    result.utm_medium = m["utm_medium"];
    result.utm_term = m["utm_term"];
    result.utm_content = m["utm_content"];
    result.utm_campaign = m["utm_campaign"];
    co_return result;
}
} // namespace core
} // namespace ee
