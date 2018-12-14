//
//  jsb_cc_ifacebook_login_delegate.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "jsb_cc_ifacebook_login_delegate.hpp"

#include "ee/Facebook.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

#include "jsb_cc_ifacebook_access_token.hpp"

namespace ee {
namespace facebook {
se::Class* __jsb_LoginDelegate_class = nullptr;
std::unordered_map<std::shared_ptr<ILoginDelegate>, se::Object*>
    __jsb_s_fbLoginDelegates;
std::vector<std::shared_ptr<ILoginDelegate>> __jsb_s_fbLoginDelegateArchive;
} // namespace facebook

namespace core {
template <>
std::shared_ptr<facebook::ILoginDelegate> get_value(const se::Value& value) {
    auto delegatePtr = static_cast<facebook::ILoginDelegate*>(
        value.toObject()->getPrivateData());
    auto iter = std::find_if(
        facebook::__jsb_s_fbLoginDelegateArchive.cbegin(),
        facebook::__jsb_s_fbLoginDelegateArchive.cend(),
        [=](const std::shared_ptr<facebook::ILoginDelegate>& ptr) -> bool {
            return delegatePtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbLoginDelegateArchive.cend()) {
        return *iter;
    } else {
        static std::shared_ptr<facebook::ILoginDelegate> defaultPtr;
        return defaultPtr;
    }
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::ILoginDelegate> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (facebook::__jsb_s_fbLoginDelegates.count(input) != 0) {
            obj = facebook::__jsb_s_fbLoginDelegates.at(input);
        } else {
            facebook::__jsb_s_fbLoginDelegateArchive.push_back(input);
            obj = se::Object::createObjectWithClass(
                facebook::__jsb_LoginDelegate_class);
            obj->setPrivateData(input.get());
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
bool jsb_finalize<facebook::ILoginDelegate>(se::State& s) {
    auto delegatePtr =
        static_cast<facebook::ILoginDelegate*>(s.nativeThisObject());
    auto iter = std::find_if(
        facebook::__jsb_s_fbLoginDelegateArchive.cbegin(),
        facebook::__jsb_s_fbLoginDelegateArchive.cend(),
        [=](const std::shared_ptr<facebook::ILoginDelegate>& ptr) -> bool {
            return delegatePtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbLoginDelegateArchive.cend()) {
        facebook::__jsb_s_fbLoginDelegateArchive.erase(iter);
    } else {
        delete delegatePtr;
    }
    return true;
}
} // namespace core

namespace facebook {
const auto jsb_LoginDelegate_finalize = &ee::core::jsb_finalize<ILoginDelegate>;
const auto jsb_LoginDelegate_onSuccess =
    &ee::core::jsb_set_callback<ILoginDelegate, &ILoginDelegate::onSuccess,
                                const std::shared_ptr<IAccessToken>&>;
const auto jsb_LoginDelegate_onFailure =
    &ee::core::jsb_set_callback<ILoginDelegate, &ILoginDelegate::onFailure,
                                const std::string&>;
const auto jsb_LoginDelegate_onCancel =
    &ee::core::jsb_set_callback<ILoginDelegate, &ILoginDelegate::onCancel>;

SE_BIND_FINALIZE_FUNC(jsb_LoginDelegate_finalize);
SE_BIND_FUNC(jsb_LoginDelegate_onSuccess);
SE_BIND_FUNC(jsb_LoginDelegate_onFailure);
SE_BIND_FUNC(jsb_LoginDelegate_onCancel);

bool register_ifacebook_login_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("ILoginDelegate", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_LoginDelegate_finalize));
    cls->defineFunction("onSuccess", _SE(jsb_LoginDelegate_onSuccess));
    cls->defineFunction("onFailure", _SE(jsb_LoginDelegate_onFailure));
    cls->defineFunction("onCancel", _SE(jsb_LoginDelegate_onCancel));

    cls->install();

    JSBClassType::registerClass<ILoginDelegate>(cls);

    __jsb_LoginDelegate_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
