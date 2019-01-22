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
               std::shared_ptr<facebook::ILoginDelegate>& input) {
    handler->setValue(value, input);
}

template <>
void set_value(se::Value& value, facebook::ILoginDelegate& input) {
    set_value_from_pointer(value, &input);
}

template <>
bool jsb_finalize<facebook::ILoginDelegate>(se::State& state) {
    return handler->finalize(state);
}
} // namespace core

namespace facebook {
namespace {
using Self = ILoginDelegate;

// clang-format off
constexpr auto finalize  = &core::makeFinalize<Self>;
constexpr auto onSuccess = &core::makeInstanceMethod<&Self::onSuccess>;
constexpr auto onFailure = &core::makeInstanceMethod<&Self::onFailure>;
constexpr auto onCancel  = &core::makeInstanceMethod<&Self::onCancel>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(onSuccess);
SE_BIND_FUNC(onFailure);
SE_BIND_FUNC(onCancel);
} // namespace

bool register_ifacebook_login_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("ILoginDelegate", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, onSuccess);
    EE_JSB_DEFINE_FUNCTION(cls, onFailure);
    EE_JSB_DEFINE_FUNCTION(cls, onCancel);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    core::handler = core::SharedPtrHandler<Self>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
