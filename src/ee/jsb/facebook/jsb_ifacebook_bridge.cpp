//
//  jsb_facebook_bridge.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/facebook/jsb_ifacebook_bridge.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

#include "ee/Facebook.hpp"

#include "jsb_facebook_graph_request.hpp"
#include "jsb_facebook_request_content.hpp"
#include "jsb_ifacebook_access_token.hpp"
#include "jsb_ifacebook_login_delegate.hpp"
#include "jsb_ifacebook_share_delegate.hpp"

namespace ee {
namespace facebook {
se::Class* __jsb_IFacebookBridge_class = nullptr;
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
            obj = se::Object::createObjectWithClass(
                facebook::__jsb_IFacebookBridge_class);
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
const auto jsb_IFacebookBridge_finalize = &ee::core::jsb_finalize<IBridge>;
const auto jsb_IFacebookBridge_isLoggedIn =
    &ee::core::jsb_method_get<IBridge, &IBridge::isLoggedIn, bool>;
const auto jsb_IFacebookBridge_logIn =
    &ee::core::jsb_method_call<IBridge, &IBridge::logIn,
                               const std::vector<std::string>&,
                               const std::shared_ptr<ILoginDelegate>&>;
const auto jsb_IFacebookBridge_createLoginDelegate =
    &ee::core::jsb_method_get<IBridge, &IBridge::createLoginDelegate,
                              std::shared_ptr<ILoginDelegate>>;
const auto jsb_IFacebookBridge_logOut =
    &ee::core::jsb_method_call<IBridge, &IBridge::logOut>;
const auto jsb_IFacebookBridge_getAccessToken =
    &ee::core::jsb_method_get<IBridge, &IBridge::getAccessToken,
                              std::shared_ptr<IAccessToken>>;
const auto jsb_IFacebookBridge_shareLinkContent =
    &ee::core::jsb_method_call<IBridge, &IBridge::shareLinkContent,
                               const std::string&,
                               const std::shared_ptr<IShareDelegate>&>;
const auto jsb_IFacebookBridge_sharePhotoContent =
    &ee::core::jsb_method_call<IBridge, &IBridge::sharePhotoContent,
                               const std::string&,
                               const std::shared_ptr<IShareDelegate>&>;
const auto jsb_IFacebookBridge_shareVideoContent =
    &ee::core::jsb_method_call<IBridge, &IBridge::shareVideoContent,
                               const std::string&,
                               const std::shared_ptr<IShareDelegate>&>;
const auto jsb_IFacebookBridge_createShareDelegate =
    &ee::core::jsb_method_get<IBridge, &IBridge::createShareDelegate,
                              std::shared_ptr<IShareDelegate>>;

SE_BIND_FINALIZE_FUNC(jsb_IFacebookBridge_finalize);
SE_BIND_FUNC(jsb_IFacebookBridge_isLoggedIn);
SE_BIND_FUNC(jsb_IFacebookBridge_logIn);
SE_BIND_FUNC(jsb_IFacebookBridge_createLoginDelegate);
SE_BIND_FUNC(jsb_IFacebookBridge_logOut);
SE_BIND_FUNC(jsb_IFacebookBridge_getAccessToken);
SE_BIND_FUNC(jsb_IFacebookBridge_shareLinkContent);
SE_BIND_FUNC(jsb_IFacebookBridge_sharePhotoContent);
SE_BIND_FUNC(jsb_IFacebookBridge_shareVideoContent);
SE_BIND_FUNC(jsb_IFacebookBridge_createShareDelegate);

se::Class* getIFacebookBridgeClass() {
    CCASSERT(__jsb_IFacebookBridge_class != nullptr,
             "__jsb_IFacebookBridge_class is null");
    return __jsb_IFacebookBridge_class;
}

bool register_ifacebook_bridge_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;

    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("IBridge", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_IFacebookBridge_finalize));

    cls->defineFunction("isLoggedIn", _SE(jsb_IFacebookBridge_isLoggedIn));
    cls->defineFunction("logIn", _SE(jsb_IFacebookBridge_logIn));
    cls->defineFunction("createLoginDelegate",
                        _SE(jsb_IFacebookBridge_createLoginDelegate));
    cls->defineFunction("logOut", _SE(jsb_IFacebookBridge_logOut));
    cls->defineFunction("getAccessToken",
                        _SE(jsb_IFacebookBridge_getAccessToken));
    cls->defineFunction("shareLinkContent",
                        _SE(jsb_IFacebookBridge_shareLinkContent));
    cls->defineFunction("sharePhotoContent",
                        _SE(jsb_IFacebookBridge_sharePhotoContent));
    cls->defineFunction("shareVideoContent",
                        _SE(jsb_IFacebookBridge_shareVideoContent));
    cls->defineFunction("createShareDelegate",
                        _SE(jsb_IFacebookBridge_createShareDelegate));

    cls->install();

    JSBClassType::registerClass<IBridge>(cls);

    __jsb_IFacebookBridge_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
