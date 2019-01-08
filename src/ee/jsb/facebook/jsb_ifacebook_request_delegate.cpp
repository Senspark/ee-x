//
//  jsb_ifacebook_request_delegate.cpp
//  Pods
//
//  Created by senspark-dev on 1/8/19.
//

#include "ee/jsb/facebook/jsb_ifacebook_request_delegate.hpp"

#include "ee/facebook/IFacebookDelegate.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace facebook {
se::Class* __jsb_RequestDelegate_class = nullptr;
std::unordered_map<std::shared_ptr<IRequestDelegate>, se::Object*>
    __jsb_s_fbRequestDelegates;
std::vector<std::shared_ptr<IRequestDelegate>> __jsb_s_fbRequestDelegateArchive;
} // namespace facebook

namespace core {
template <>
std::shared_ptr<facebook::IRequestDelegate> get_value(const se::Value& value) {
    auto delegatePtr = static_cast<facebook::IRequestDelegate*>(
        value.toObject()->getPrivateData());
    auto iter = std::find_if(
        facebook::__jsb_s_fbRequestDelegateArchive.cbegin(),
        facebook::__jsb_s_fbRequestDelegateArchive.cend(),
        [=](const std::shared_ptr<facebook::IRequestDelegate>& ptr) -> bool {
            return delegatePtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbRequestDelegateArchive.cend()) {
        return *iter;
    } else {
        return std::shared_ptr<facebook::IRequestDelegate>(delegatePtr);
    }
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IRequestDelegate> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (facebook::__jsb_s_fbRequestDelegates.count(input) != 0) {
            obj = facebook::__jsb_s_fbRequestDelegates.at(input);
        } else {
            facebook::__jsb_s_fbRequestDelegateArchive.push_back(input);
            obj = se::Object::createObjectWithClass(
                facebook::__jsb_RequestDelegate_class);
            obj->setPrivateData(input.get());
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
bool jsb_finalize<facebook::IRequestDelegate>(se::State& s) {
    auto delegatePtr =
        static_cast<facebook::IRequestDelegate*>(s.nativeThisObject());
    auto iter = std::find_if(
        facebook::__jsb_s_fbRequestDelegateArchive.cbegin(),
        facebook::__jsb_s_fbRequestDelegateArchive.cend(),
        [=](const std::shared_ptr<facebook::IRequestDelegate>& ptr) -> bool {
            return delegatePtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_fbRequestDelegateArchive.cend()) {
        facebook::__jsb_s_fbRequestDelegateArchive.erase(iter);
    } else {
        delete delegatePtr;
    }
    return true;
}
} // namespace core

namespace facebook {
const auto jsb_RequestDelegate_finalize =
    &ee::core::jsb_finalize<IFacebookRequestDelegate>;
const auto jsb_RequestDelegate_onSuccess =
    &ee::core::jsb_set_callback<IFacebookRequestDelegate,
                                &IFacebookRequestDelegate::onSuccess,
    const std::string&, const std::vector<std::string>&>;
const auto jsb_RequestDelegate_onFailure =
    &ee::core::jsb_set_callback<IFacebookRequestDelegate,
                                &IFacebookRequestDelegate::onFailure,
                                const std::string&>;
const auto jsb_RequestDelegate_onCancel =
    &ee::core::jsb_set_callback<IFacebookRequestDelegate,
                                &IFacebookRequestDelegate::onCancel>;

SE_BIND_FINALIZE_FUNC(jsb_RequestDelegate_finalize);
SE_BIND_FUNC(jsb_RequestDelegate_onSuccess);
SE_BIND_FUNC(jsb_RequestDelegate_onFailure);
SE_BIND_FUNC(jsb_RequestDelegate_onCancel);

bool register_ifacebook_request_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("IRequestDelegate", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_RequestDelegate_finalize));
    cls->defineFunction("onSuccess", _SE(jsb_RequestDelegate_onSuccess));
    cls->defineFunction("onFailure", _SE(jsb_RequestDelegate_onFailure));
    cls->defineFunction("onCancel", _SE(jsb_RequestDelegate_onCancel));

    cls->install();

    JSBClassType::registerClass<IFacebookRequestDelegate>(cls);

    __jsb_RequestDelegate_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
