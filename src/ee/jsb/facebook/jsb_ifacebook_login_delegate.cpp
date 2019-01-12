//
//  jsb_ifacebook_login_delegate.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/facebook/jsb_ifacebook_login_delegate.hpp"

#include "ee/facebook/internal/FacebookLoginDelegate.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/facebook/jsb_ifacebook_access_token.hpp"

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<facebook::ILoginDelegate>> handler;
} // namespace

template <>
std::shared_ptr<facebook::ILoginDelegate> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::ILoginDelegate> input) {
    handler->setValue(value, input);
}

template <>
bool jsb_finalize<facebook::ILoginDelegate>(se::State& s) {
    return handler->finalize(s);
}
} // namespace core

namespace facebook {
namespace {
const auto jsb_LoginDelegate_finalize = &core::jsb_finalize<ILoginDelegate>;
const auto jsb_LoginDelegate_onSuccess =
    &core::jsb_set_callback<ILoginDelegate, &ILoginDelegate::onSuccess,
                            const std::shared_ptr<IAccessToken>&>;
const auto jsb_LoginDelegate_onFailure =
    &core::jsb_set_callback<ILoginDelegate, &ILoginDelegate::onFailure,
                            const std::string&>;
const auto jsb_LoginDelegate_onCancel =
    &core::jsb_set_callback<ILoginDelegate, &ILoginDelegate::onCancel>;
} // namespace

SE_BIND_FINALIZE_FUNC(jsb_LoginDelegate_finalize);
SE_BIND_FUNC(jsb_LoginDelegate_onSuccess);
SE_BIND_FUNC(jsb_LoginDelegate_onFailure);
SE_BIND_FUNC(jsb_LoginDelegate_onCancel);

bool register_ifacebook_login_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("ILoginDelegate", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_LoginDelegate_finalize));
    cls->defineFunction("onSuccess", _SE(jsb_LoginDelegate_onSuccess));
    cls->defineFunction("onFailure", _SE(jsb_LoginDelegate_onFailure));
    cls->defineFunction("onCancel", _SE(jsb_LoginDelegate_onCancel));

    cls->install();

    JSBClassType::registerClass<ILoginDelegate>(cls);
    core::handler = core::SharedPtrHandler<ILoginDelegate>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
