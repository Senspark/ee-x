//
//  jsb_facebook_bridge.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/facebook/jsb_ifacebook_bridge.hpp"

#include "ee/Facebook.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/facebook/jsb_facebook_graph_request.hpp"
#include "ee/jsb/facebook/jsb_facebook_request_content.hpp"
#include "ee/jsb/facebook/jsb_ifacebook_access_token.hpp"
#include "ee/jsb/facebook/jsb_ifacebook_login_delegate.hpp"
#include "ee/jsb/facebook/jsb_ifacebook_share_delegate.hpp"

namespace ee {
namespace facebook {
namespace {
se::Class* clazz = nullptr;

using Self = IBridge;

// clang-format off
constexpr auto finalize              = &core::makeFinalize<Self>;
constexpr auto isLoggedIn            = &core::makeInstanceMethod<&Self::isLoggedIn>;
constexpr auto logIn                 = &core::makeInstanceMethod<&Self::logIn>;
constexpr auto createLoginDelegate   = &core::makeInstanceMethod<&Self::createLoginDelegate>;
constexpr auto logOut                = &core::makeInstanceMethod<&Self::logOut>;
constexpr auto getAccessToken        = &core::makeInstanceMethod<&Self::getAccessToken>;
constexpr auto shareLinkContent      = &core::makeInstanceMethod<&Self::shareLinkContent>;
constexpr auto sharePhotoContent     = &core::makeInstanceMethod<&Self::sharePhotoContent>;
constexpr auto shareVideoContent     = &core::makeInstanceMethod<&Self::shareVideoContent>;
constexpr auto createShareDelegate   = &core::makeInstanceMethod<&Self::createShareDelegate>;
constexpr auto graphRequest          = &core::makeInstanceMethod<&Self::graphRequest>;
constexpr auto createGraphDelegate   = &core::makeInstanceMethod<&Self::createGraphDelegate>;
constexpr auto sendRequest           = &core::makeInstanceMethod<&Self::sendRequest>;
constexpr auto createRequestDelegate = &core::makeInstanceMethod<&Self::createRequestDelegate>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(isLoggedIn);
SE_BIND_FUNC(logIn);
SE_BIND_FUNC(createLoginDelegate);
SE_BIND_FUNC(logOut);
SE_BIND_FUNC(getAccessToken);
SE_BIND_FUNC(shareLinkContent);
SE_BIND_FUNC(sharePhotoContent);
SE_BIND_FUNC(shareVideoContent);
SE_BIND_FUNC(createShareDelegate);
SE_BIND_FUNC(graphRequest);
SE_BIND_FUNC(createGraphDelegate);
SE_BIND_FUNC(sendRequest);
SE_BIND_FUNC(createRequestDelegate);
} // namespace

se::Class* getIFacebookBridgeClass() {
    CCASSERT(clazz != nullptr, "__jsb_IFacebookBridge_class is null");
    return clazz;
}

bool register_ifacebook_bridge_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;

    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("IBridge", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, isLoggedIn);
    EE_JSB_DEFINE_FUNCTION(cls, logIn);
    EE_JSB_DEFINE_FUNCTION(cls, createLoginDelegate);
    EE_JSB_DEFINE_FUNCTION(cls, logOut);
    EE_JSB_DEFINE_FUNCTION(cls, getAccessToken);
    EE_JSB_DEFINE_FUNCTION(cls, shareLinkContent);
    EE_JSB_DEFINE_FUNCTION(cls, sharePhotoContent);
    EE_JSB_DEFINE_FUNCTION(cls, shareVideoContent);
    EE_JSB_DEFINE_FUNCTION(cls, createShareDelegate);
    EE_JSB_DEFINE_FUNCTION(cls, graphRequest);
    EE_JSB_DEFINE_FUNCTION(cls, createGraphDelegate);
    EE_JSB_DEFINE_FUNCTION(cls, sendRequest);
    EE_JSB_DEFINE_FUNCTION(cls, createRequestDelegate);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
