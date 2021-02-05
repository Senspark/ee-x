//
//  FirebaseRemoteConfig.cpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase_remote_config/internal/FirebaseRemoteConfigBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/JsonUtils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/firebase_remote_config/FirebaseFetchStatus.hpp"

namespace ee {
namespace firebase {
namespace remote_config {
namespace {
const std::string kPrefix = "FirebaseRemoteConfigBridge";
const std::string kInitialize = kPrefix + "Initialize";
const std::string kSetSettings = kPrefix + "SetSettings";
const std::string kFetch = kPrefix + "Fetch";
const std::string kActivate = kPrefix + "Activate";
const std::string kSetDefaults = kPrefix + "SetDefaults";
const std::string kGetBool = kPrefix + "GetBool";
const std::string kGetLong = kPrefix + "GetLong";
const std::string kGetDouble = kPrefix + "GetDouble";
const std::string kGetString = kPrefix + "GetString";
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

Task<bool> Self::initialize() {
    auto response = co_await bridge_.callAsync(kInitialize);
    co_return core::toBool(response);
}

Task<> Self::setSettings(std::int64_t fetchTimeOut,
                         std::int64_t fetchInterval) {
    nlohmann::json request;
    request["fetchTimeOut"] = fetchTimeOut;
    request["fetchInterval"] = fetchInterval;
    co_await bridge_.callAsync(kSetSettings, request.dump());
}

Task<FetchStatus> Self::fetch(std::int64_t fetchTimeOut) {
    nlohmann::json request;
    request["fetchTimeOut"] = fetchTimeOut;
    auto response = co_await bridge_.callAsync(kFetch, request.dump());
    co_return static_cast<FetchStatus>(std::stoi(response));
}

Task<bool> Self::activate() {
    auto response = co_await bridge_.callAsync(kActivate);
    co_return core::toBool(response);
}

Task<> Self::setDefaults(
    const std::unordered_map<
        std::string, std::variant<bool, std::int64_t, double, std::string>>&
        defaults) {
    nlohmann::json request;
    request["defaults"] = defaults;
    co_await bridge_.callAsync(kSetDefaults, request.dump());
}

Task<bool> Self::getBool(const std::string& key) {
    auto response = co_await bridge_.callAsync(kGetBool, key);
    co_return core::toBool(response);
}

Task<std::int64_t> Self::getLong(const std::string& key) {
    auto response = co_await bridge_.callAsync(kGetLong, key);
    co_return std::stoll(response);
}

Task<double> Self::getDouble(const std::string& key) {
    auto response = co_await bridge_.callAsync(kGetDouble, key);
    co_return std::stod(response);
}

Task<std::string> Self::getString(const std::string& key) {
    auto response = co_await bridge_.callAsync(kGetString, key);
    co_return response;
}
} // namespace remote_config
} // namespace firebase
} // namespace ee
