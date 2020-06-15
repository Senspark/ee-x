//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/facebook/private/FacebookBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/core/LogLevel.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>
#include <ee/core/internal/SharedPtrUtils.hpp>

#include "ee/facebook/FacebookGraphRequest.hpp"
#include "ee/facebook/FacebookRequestContent.hpp"
#include "ee/facebook/private/FacebookAccessToken.hpp"
#include "ee/facebook/private/FacebookGraphDelegate.hpp"
#include "ee/facebook/private/FacebookLoginDelegate.hpp"
#include "ee/facebook/private/FacebookRequestDelegate.hpp"
#include "ee/facebook/private/FacebookShareDelegate.hpp"

namespace ee {
namespace core {
template <>
std::unique_ptr<IFacebook> PluginManager::createPlugin() {
    return std::make_unique<facebook::Bridge>();
}
} // namespace core

namespace facebook {
namespace {
// clang-format off
constexpr auto k__registerNotifications = "Facebook_registerNotifications";
constexpr auto k__isLoggedIn            = "Facebook_isLoggedIn";
constexpr auto k__logIn                 = "Facebook_logIn";
constexpr auto k__logOut                = "Facebook_logOut";
constexpr auto k__getAccessToken        = "Facebook_getAccessToken";
constexpr auto k__onProfileChanged      = "Facebook_onProfileChanged";
constexpr auto k__graphRequest          = "Facebook_graphRequest";
constexpr auto k__sendRequest           = "Facebook_sendRequest";
constexpr auto k__shareLinkContent      = "Facebook_shareLinkContent";
constexpr auto k__sharePhotoContent     = "Facebook_sharePhotoContent";
constexpr auto k__shareVideoContent     = "Facebook_shareVideoContent";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge()
    : bridge_(MessageBridge::getInstance()) {
    PluginManager::addPlugin(Plugin::Facebook);
    delegateId_ = 0;
    bridge_.registerHandler(
        [this](const std::string& message) {
            onProfileChanged(message);
            return "";
        },
        k__onProfileChanged);
    registerNotifications();
}

Self::~Bridge() = default;

void Self::destroy() {
    bridge_.deregisterHandler(k__onProfileChanged);
    PluginManager::removePlugin(Plugin::Facebook);
}

void Self::registerNotifications() {
    bridge_.call(k__registerNotifications);
}

void Self::onProfileChanged(const std::string& profile) {
    //
}

bool Self::isLoggedIn() const {
    auto response = bridge_.call(k__isLoggedIn);
    return core::toBool(response);
}

void Self::logIn(const std::vector<std::string>& permissions,
                 const std::shared_ptr<ILoginDelegate>& delegate_) {
    auto&& delegate = std::dynamic_pointer_cast<LoginDelegate>(delegate_);
    delegate->self_ = delegate;
    nlohmann::json json;
    json["permissions"] = permissions;
    json["tag"] = delegate->tag_;
    bridge_.call(k__logIn, json.dump());
}

std::shared_ptr<ILoginDelegate> Self::createLoginDelegate() {
    return core::makeShared<LoginDelegate>(bridge_, delegateId_++);
}

void Self::logOut() {
    bridge_.call(k__logOut);
}

std::shared_ptr<IAccessToken> Self::getAccessToken() const {
    auto response = bridge_.call(k__getAccessToken);
    if (response.empty()) {
        return nullptr;
    }
    auto token = core::makeShared<AccessToken>(response);
    return token;
}

void Self::graphRequest(const GraphRequest& request,
                        const std::shared_ptr<IGraphDelegate>& delegate_) {
    auto&& delegate = std::dynamic_pointer_cast<GraphDelegate>(delegate_);
    delegate->self_ = delegate;
    auto json = nlohmann::json::parse(request.toString());
    json["tag"] = delegate->tag_;
    bridge_.call(k__graphRequest, json.dump());
}

std::shared_ptr<IGraphDelegate> Self::createGraphDelegate() {
    return core::makeShared<GraphDelegate>(bridge_, delegateId_++);
}

void Self::sendRequest(const RequestContent& content,
                       const std::shared_ptr<IRequestDelegate>& delegate_) {
    auto&& delegate = std::dynamic_pointer_cast<RequestDelegate>(delegate_);
    delegate->self_ = delegate;
    auto json = nlohmann::json::parse(content.toString());
    json["tag"] = delegate->tag_;
    bridge_.call(k__sendRequest, json.dump());
}

std::shared_ptr<IRequestDelegate> Self::createRequestDelegate() {
    return core::makeShared<RequestDelegate>(bridge_, delegateId_++);
}

void Self::shareLinkContent(const std::string& url,
                            const std::shared_ptr<IShareDelegate>& delegate_) {
    auto&& delegate = std::dynamic_pointer_cast<ShareDelegate>(delegate_);
    delegate->self_ = delegate;
    nlohmann::json json;
    json["url"] = url;
    json["tag"] = delegate->tag_;
    bridge_.call(k__shareLinkContent, json.dump());
}

void Self::sharePhotoContent(const std::string& url,
                             const std::shared_ptr<IShareDelegate>& delegate_) {
    auto&& delegate = std::dynamic_pointer_cast<ShareDelegate>(delegate_);
    delegate->self_ = delegate;
    nlohmann::json json;
    json["url"] = url;
    json["tag"] = delegate->tag_;
    bridge_.call(k__sharePhotoContent, json.dump());
}

void Self::shareVideoContent(const std::string& url,
                             const std::shared_ptr<IShareDelegate>& delegate_) {
    auto&& delegate = std::dynamic_pointer_cast<ShareDelegate>(delegate_);
    delegate->self_ = delegate;
    nlohmann::json json;
    json["url"] = url;
    json["tag"] = delegate->tag_;
    bridge_.call(k__shareVideoContent, json.dump());
}

std::shared_ptr<IShareDelegate> Self::createShareDelegate() {
    return core::makeShared<ShareDelegate>(bridge_, delegateId_++);
}
} // namespace facebook
} // namespace ee
