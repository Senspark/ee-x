//
//  jsb_ifacebook_share_delegate.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/facebook/jsb_ifacebook_share_delegate.hpp"

#include "ee/facebook/internal/FacebookShareDelegate.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<facebook::IShareDelegate>> handler;
} // namespace

template <>
std::shared_ptr<facebook::IShareDelegate> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IShareDelegate> input) {
    handler->setValue(value, input);
}

template <>
bool jsb_finalize<facebook::IShareDelegate>(se::State& s) {
    return handler->finalize(s);
}
} // namespace core

namespace facebook {
namespace {
const auto jsb_ShareDelegate_finalize =
    &core::jsb_finalize<IFacebookShareDelegate>;
const auto jsb_ShareDelegate_onSuccess =
    &core::jsb_set_callback<IFacebookShareDelegate,
                            &IFacebookShareDelegate::onSuccess,
                            const std::string&>;
const auto jsb_ShareDelegate_onFailure =
    &core::jsb_set_callback<IFacebookShareDelegate,
                            &IFacebookShareDelegate::onFailure,
                            const std::string&>;
const auto jsb_ShareDelegate_onCancel =
    &core::jsb_set_callback<IFacebookShareDelegate,
                            &IFacebookShareDelegate::onCancel>;
} // namespace

SE_BIND_FINALIZE_FUNC(jsb_ShareDelegate_finalize);
SE_BIND_FUNC(jsb_ShareDelegate_onSuccess);
SE_BIND_FUNC(jsb_ShareDelegate_onFailure);
SE_BIND_FUNC(jsb_ShareDelegate_onCancel);

bool register_ifacebook_share_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("IShareDelegate", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_ShareDelegate_finalize));
    cls->defineFunction("onSuccess", _SE(jsb_ShareDelegate_onSuccess));
    cls->defineFunction("onFailure", _SE(jsb_ShareDelegate_onFailure));
    cls->defineFunction("onCancel", _SE(jsb_ShareDelegate_onCancel));

    cls->install();

    JSBClassType::registerClass<ShareDelegate>(cls);
    core::handler = core::SharedPtrHandler<IShareDelegate>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
