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
} // namespace
std::unordered_map<std::shared_ptr<facebook::IBridge>, se::Object*>
    __jsb_s_fbBridges;
std::vector<std::shared_ptr<facebook::IBridge>> __jsb_s_fbBridgeArchive;
} // namespace facebook

namespace core {
template <>
std::shared_ptr<facebook::IBridge> get_value(const se::Value& value) {
    auto itemPtr =
        static_cast<facebook::IBridge*>(value.toObject()->getPrivateData());
    auto iter = std::find_if(
        facebook::__jsb_s_fbBridgeArchive.cbegin(),
        facebook::__jsb_s_fbBridgeArchive.cend(),
        [=](const std::shared_ptr<facebook::IBridge>& ptr) -> bool {
            return itemPtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbBridgeArchive.cend()) {
        return *iter;
    } else {
        return std::shared_ptr<facebook::IBridge>(itemPtr);
    }
}

template <>
void set_value(se::Value& value, std::shared_ptr<facebook::IBridge> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (facebook::__jsb_s_fbBridges.count(input) != 0) {
            obj = facebook::__jsb_s_fbBridges.at(input);
        } else {
            facebook::__jsb_s_fbBridgeArchive.push_back(input);
            obj = se::Object::createObjectWithClass(facebook::clazz);
            obj->setPrivateData(input.get());
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
bool jsb_finalize<facebook::IBridge>(se::State& s) {
    auto itemPtr = static_cast<facebook::IBridge*>(s.nativeThisObject());
    auto iter = std::find_if(
        facebook::__jsb_s_fbBridgeArchive.cbegin(),
        facebook::__jsb_s_fbBridgeArchive.cend(),
        [=](const std::shared_ptr<facebook::IBridge>& ptr) -> bool {
            return itemPtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbBridgeArchive.cend()) {
        facebook::__jsb_s_fbBridgeArchive.erase(iter);
    } else {
        delete itemPtr;
    }
    return true;
}
} // namespace core

namespace facebook {
namespace {
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
    cls->defineFunction("isLoggedIn", _SE(isLoggedIn));
    cls->defineFunction("logIn", _SE(logIn));
    cls->defineFunction("createLoginDelegate", _SE(createLoginDelegate));
    cls->defineFunction("logOut", _SE(logOut));
    cls->defineFunction("getAccessToken", _SE(getAccessToken));
    cls->defineFunction("shareLinkContent", _SE(shareLinkContent));
    cls->defineFunction("sharePhotoContent", _SE(sharePhotoContent));
    cls->defineFunction("shareVideoContent", _SE(shareVideoContent));
    cls->defineFunction("createShareDelegate", _SE(createShareDelegate));
    cls->defineFunction("graphRequest", _SE(graphRequest));
    cls->defineFunction("createGraphDelegate", _SE(createGraphDelegate));
    cls->defineFunction("sendRequest", _SE(sendRequest));
    cls->defineFunction("createRequestDelegate", _SE(createRequestDelegate));
    cls->install();

    JSBClassType::registerClass<IBridge>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
