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
void set_value(se::Value& value,
               std::shared_ptr<facebook::IShareDelegate>& input) {
    handler->setValue(value, input);
}

template <>
bool jsb_finalize<facebook::IShareDelegate>(se::State& s) {
    return handler->finalize(s);
}
} // namespace core

namespace facebook {
namespace {
using Self = IShareDelegate;
// clang-format off
constexpr auto finalize  = &core::makeFinalize<Self>;
constexpr auto onSuccess = &core::jsb_set_callback<IShareDelegate, &Self::onSuccess, const std::string&>;
constexpr auto onFailure = &core::jsb_set_callback<IShareDelegate, &Self::onFailure, const std::string&>;
constexpr auto onCancel  = &core::jsb_set_callback<IShareDelegate, &Self::onCancel>;
// clang-format on
} // namespace

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(onSuccess);
SE_BIND_FUNC(onFailure);
SE_BIND_FUNC(onCancel);

bool register_ifacebook_share_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("IShareDelegate", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));
    cls->defineFunction("onSuccess", _SE(onSuccess));
    cls->defineFunction("onFailure", _SE(onFailure));
    cls->defineFunction("onCancel", _SE(onCancel));

    cls->install();

    JSBClassType::registerClass<IShareDelegate>(cls);
    core::handler = core::SharedPtrHandler<IShareDelegate>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
