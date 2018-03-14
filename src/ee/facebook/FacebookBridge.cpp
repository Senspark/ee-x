//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/facebook/FacebookBridge.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
namespace {
// clang-format off
constexpr auto k__registerNotifications = "Facebook_registerNotifications";
constexpr auto k__isLoggedIn            = "Facebook_isLoggedIn";
constexpr auto k__logIn                 = "Facebook_logIn";
constexpr auto k__logOut                = "Facebook_logOut";
constexpr auto k__getAccessToken        = "Facebook_getAccessToken";
constexpr auto k__getUserId             = "Facebook_getUserId";
constexpr auto k__onLoginResult         = "Facebook_onLoginResult";
constexpr auto k__onProfileChanged      = "Facebook_onProfileChanged";
constexpr auto k__shareLinkContent      = "Facebook_shareLinkContent";
constexpr auto k__sharePhotoContent     = "Facebook_sharePhotoContent";
constexpr auto k__shareVideoContent     = "Facebook_shareVideoContent";
constexpr auto k__onShareResult         = "Facebook_onShareResult";
// clang-format on
} // namespace

using Self = Facebook;

Self::Facebook()
    : bridge_(MessageBridge::getInstance()) {
    isLoggingIn_ = false;
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto result = core::toBool(message);
            onLoginResult(result);
            return "";
        },
        k__onLoginResult);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onProfileChanged(message);
            return "";
        },
        k__onProfileChanged);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto result = core::toBool(message);
            onShareResult(result);
            return "";
        },
        k__onShareResult);
    registerNotifications();
}

Self::~Facebook() {
    bridge_.deregisterHandler(k__onShareResult);
}

void Self::registerNotifications() {
    bridge_.call(k__registerNotifications);
}

void Self::onLoginResult(bool result) {
    if (result) {
        // Waiting for profile to be updated.
    } else {
        // Failed.
        if (loginCallback_) {
            loginCallback_(result);
        }
        isLoggingIn_ = false;
    }
}

void Self::onProfileChanged(const std::string& profile) {
    if (isLoggingIn_) {
        if (loginCallback_) {
            loginCallback_(true);
        }
    }
}

void Self::onShareResult(bool result) {
    if (shareCallback_) {
        shareCallback_(result);
    }
}

bool Self::isLoggedIn() const {
    auto response = bridge_.call(k__isLoggedIn);
    return core::toBool(response);
}

void Self::logIn(const std::vector<std::string>& permissions) {
    isLoggingIn_ = true;
    bridge_.call(k__logIn, nlohmann::json(permissions).dump());
}

void Self::logOut() {
    bridge_.call(k__logOut);
}

std::string Self::getAccessToken() const {
    return bridge_.call(k__getAccessToken);
}

std::string Self::getUserId() const {
    return bridge_.call(k__getUserId);
}

void Self::setLoginCallback(const LoginCallback& callback) {
    loginCallback_ = callback;
}

void Self::shareLinkContent(const std::string& url) {
    bridge_.call(k__shareLinkContent, url);
}

void Self::sharePhotoContent(const std::string& name) {
    bridge_.call(k__sharePhotoContent, name);
}

void Self::shareVideoContent(const std::string& name) {
    bridge_.call(k__shareVideoContent, name);
}

void Self::setShareCallback(const ShareCallback& callback) {
    shareCallback_ = callback;
}
} // namespace facebook
} // namespace ee
