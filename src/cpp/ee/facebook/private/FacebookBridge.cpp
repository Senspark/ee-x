//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/facebook/private/FacebookBridge.hpp"

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/facebook/FacebookGraphRequest.hpp"
#include "ee/facebook/FacebookRequestContent.hpp"
#include "ee/facebook/private/FacebookAccessToken.hpp"

namespace ee {
namespace facebook {
namespace {
// clang-format off
const std::string kPrefix         = "FacebookBridge";
const auto kRegisterNotifications = kPrefix + "RegisterNotifications";
const auto kIsLoggedIn            = kPrefix + "IsLoggedIn";
const auto kLogIn                 = kPrefix + "LogIn";
const auto kLogOut                = kPrefix + "LogOut";
const auto kGetAccessToken        = kPrefix + "GetAccessToken";
const auto kOnProfileChanged      = kPrefix + "OnProfileChanged";
const auto kGraphRequest          = kPrefix + "GraphRequest";
const auto kSendRequest           = kPrefix + "SendRequest";
const auto kShareContent          = kPrefix + "ShareContent";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer) {
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onProfileChanged(message);
        },
        kOnProfileChanged);
    registerNotifications();
}

Self::~Bridge() = default;

void Self::destroy() {
    bridge_.deregisterHandler(kOnProfileChanged);
    destroyer_();
}

void Self::registerNotifications() {
    bridge_.call(kRegisterNotifications);
}

void Self::onProfileChanged(const std::string& profile) {
    //
}

bool Self::isLoggedIn() const {
    auto response = bridge_.call(kIsLoggedIn);
    return core::toBool(response);
}

Task<LoginResult> Self::logIn(const std::vector<std::string>& permissions) {
    nlohmann::json request;
    request["permissions"] = permissions;
    auto response = co_await bridge_.callAsync(kLogIn, request.dump());
    auto json = nlohmann::json::parse(response);
    LoginResult result = {
        .successful = json["successful"],
        .canceled = json["canceled"],
        .errorMessage = json["errorMessage"],
    };
    co_return result;
}

Task<> Self::logOut() {
    co_await bridge_.callAsync(kLogOut);
}

std::shared_ptr<IAccessToken> Self::getAccessToken() const {
    auto response = bridge_.call(kGetAccessToken);
    if (response.empty()) {
        return nullptr;
    }
    auto token = std::make_shared<AccessToken>(response);
    return token;
}

Task<GraphResult> Self::graphRequest(const GraphRequest& request) {
    auto response =
        co_await bridge_.callAsync(kGraphRequest, request.toString());
    auto json = nlohmann::json::parse(response);
    GraphResult result = {
        .successful = json["successful"],
        .response = json["response"],
        .errorMessage = json["errorMessage"],
    };
    co_return result;
}

Task<ShareResult> Self::shareContent(ShareType type, const std::string& url) {
    nlohmann::json request;
    request["type"] = static_cast<int>(type);
    request["url"] = url;
    auto response = co_await bridge_.callAsync(kShareContent, request.dump());
    auto json = nlohmann::json::parse(response);
    ShareResult result = {
        .successful = json["successful"],
        .canceled = json["canceled"],
        .errorMessage = json["errorMessage"],
    };
    co_return result;
}
} // namespace facebook
} // namespace ee
