//
//  jsb_ifacebook_request_delegate.cpp
//  Pods
//
//  Created by senspark-dev on 1/8/19.
//

#include "ee/jsb/facebook/jsb_ifacebook_request_delegate.hpp"

#include "ee/facebook/internal/FacebookRequestDelegate.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<facebook::IRequestDelegate>> handler;
} // namespace

template <>
std::shared_ptr<facebook::IRequestDelegate> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IRequestDelegate> input) {
    handler->setValue(value, input);
}

template <>
bool jsb_finalize<facebook::IRequestDelegate>(se::State& s) {
    return handler->finalize(s);
}
} // namespace core

namespace facebook {
namespace {
const auto jsb_RequestDelegate_finalize =
    &core::jsb_finalize<IFacebookRequestDelegate>;
const auto jsb_RequestDelegate_onSuccess = &core::jsb_set_callback<
    IFacebookRequestDelegate, &IFacebookRequestDelegate::onSuccess,
    const std::string&, const std::vector<std::string>&>;
const auto jsb_RequestDelegate_onFailure =
    &core::jsb_set_callback<IFacebookRequestDelegate,
                            &IFacebookRequestDelegate::onFailure,
                            const std::string&>;
const auto jsb_RequestDelegate_onCancel =
    &core::jsb_set_callback<IFacebookRequestDelegate,
                            &IFacebookRequestDelegate::onCancel>;
} // namespace

SE_BIND_FINALIZE_FUNC(jsb_RequestDelegate_finalize);
SE_BIND_FUNC(jsb_RequestDelegate_onSuccess);
SE_BIND_FUNC(jsb_RequestDelegate_onFailure);
SE_BIND_FUNC(jsb_RequestDelegate_onCancel);

bool register_ifacebook_request_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("IRequestDelegate", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_RequestDelegate_finalize));
    cls->defineFunction("onSuccess", _SE(jsb_RequestDelegate_onSuccess));
    cls->defineFunction("onFailure", _SE(jsb_RequestDelegate_onFailure));
    cls->defineFunction("onCancel", _SE(jsb_RequestDelegate_onCancel));

    cls->install();

    JSBClassType::registerClass<RequestDelegate>(cls);
    core::handler = core::SharedPtrHandler<IRequestDelegate>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
