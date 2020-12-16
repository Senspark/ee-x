//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/facebook/private/FacebookBridge.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/LogLevel.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/SharedPtrUtils.hpp>
#include <ee/nlohmann/json.hpp>

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
std::shared_ptr<IFacebook>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    return std::make_shared<facebook::Bridge>(bridge);
}
} // namespace core

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
const auto kShareLinkContent      = kPrefix + "ShareLinkContent";
const auto kSharePhotoContent     = kPrefix + "SharePhotoContent";
const auto kShareVideoContent     = kPrefix + "ShareVideoContent";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge) {
    PluginManager::addPlugin(Plugin::Facebook);
    delegateId_ = 0;
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this, message] { //
                onProfileChanged(message);
            });
            return "";
        },
        kOnProfileChanged);
    registerNotifications();
}

Self::~Bridge() = default;

void Self::destroy() {
    bridge_.deregisterHandler(kOnProfileChanged);
    PluginManager::removePlugin(Plugin::Facebook);
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

void Self::logIn(const std::vector<std::string>& permissions,
                 const std::shared_ptr<ILoginDelegate>& delegate_) {
    auto&& delegate = std::dynamic_pointer_cast<LoginDelegate>(delegate_);
    delegate->self_ = delegate;
    nlohmann::json json;
    json["permissions"] = permissions;
    json["tag"] = delegate->tag_;
    bridge_.call(kLogIn, json.dump());
}

std::shared_ptr<ILoginDelegate> Self::createLoginDelegate() {
    return core::makeShared<LoginDelegate>(bridge_, delegateId_++);
}

void Self::logOut() {
    bridge_.call(kLogOut);
}

std::shared_ptr<IAccessToken> Self::getAccessToken() const {
    auto response = bridge_.call(kGetAccessToken);
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
    bridge_.call(kGraphRequest, json.dump());
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
    bridge_.call(kSendRequest, json.dump());
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
    bridge_.call(kShareLinkContent, json.dump());
}

void Self::sharePhotoContent(const std::string& url,
                             const std::shared_ptr<IShareDelegate>& delegate_) {
    auto&& delegate = std::dynamic_pointer_cast<ShareDelegate>(delegate_);
    delegate->self_ = delegate;
    nlohmann::json json;
    json["url"] = url;
    json["tag"] = delegate->tag_;
    bridge_.call(kSharePhotoContent, json.dump());
}

void Self::shareVideoContent(const std::string& url,
                             const std::shared_ptr<IShareDelegate>& delegate_) {
    auto&& delegate = std::dynamic_pointer_cast<ShareDelegate>(delegate_);
    delegate->self_ = delegate;
    nlohmann::json json;
    json["url"] = url;
    json["tag"] = delegate->tag_;
    bridge_.call(kShareVideoContent, json.dump());
}

std::shared_ptr<IShareDelegate> Self::createShareDelegate() {
    return core::makeShared<ShareDelegate>(bridge_, delegateId_++);
}
} // namespace facebook
} // namespace ee
