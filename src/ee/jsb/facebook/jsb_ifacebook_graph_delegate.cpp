//
//  jsb_ifacebook_graph_delegate.cpp
//  Pods
//
//  Created by senspark-dev on 1/8/19.
//

#include "ee/jsb/facebook/jsb_ifacebook_graph_delegate.hpp"

#include "ee/facebook/internal/FacebookGraphDelegate.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

using Self = ee::facebook::IGraphDelegate;

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<Self>> handler;
} // namespace

template <>
std::shared_ptr<Self> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value, std::shared_ptr<Self>& input) {
    handler->setValue(value, input);
}

template <>
void set_value(se::Value& value, Self& input) {
    set_value_from_pointer(value, &input);
}

template <>
bool makeFinalize<Self>(se::State& state) {
    return handler->finalize(state);
}
} // namespace core

namespace facebook {
namespace {
// clang-format off
constexpr auto finalize  = &core::makeFinalize<Self>;
constexpr auto onSuccess = &core::makeMethod<&Self::onSuccess>;
constexpr auto onFailure = &core::makeMethod<&Self::onFailure>;
constexpr auto onCancel  = &core::makeMethod<&Self::onCancel>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(onSuccess);
SE_BIND_FUNC(onFailure);
SE_BIND_FUNC(onCancel);
} // namespace

bool register_ifacebook_graph_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("IGraphDelegate", facebookObj, nullptr, nullptr);
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
