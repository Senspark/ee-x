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
void set_value(se::Value& value,
               std::shared_ptr<facebook::IRequestDelegate>& input) {
    handler->setValue(value, input);
}

template <>
bool jsb_finalize<facebook::IRequestDelegate>(se::State& state) {
    return handler->finalize(state);
}
} // namespace core

namespace facebook {
namespace {
using Self = IRequestDelegate;

// clang-format off
constexpr auto finalize  = &core::makeFinalize<Self>;
constexpr auto onSuccess = &core::jsb_set_callback<Self, &Self::onSuccess, const std::string&, const std::vector<std::string>&>;
constexpr auto onFailure = &core::jsb_set_callback<Self, &Self::onFailure, const std::string&>;
constexpr auto onCancel  = &core::jsb_set_callback<Self, &Self::onCancel>;
// clang-format on
} // namespace

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(onSuccess);
SE_BIND_FUNC(onFailure);
SE_BIND_FUNC(onCancel);

bool register_ifacebook_request_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("IRequestDelegate", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, onSuccess);
    EE_JSB_DEFINE_FUNCTION(cls, onFailure);
    EE_JSB_DEFINE_FUNCTION(cls, onCancel);

    cls->install();

    JSBClassType::registerClass<RequestDelegate>(cls);
    core::handler = core::SharedPtrHandler<Self>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
