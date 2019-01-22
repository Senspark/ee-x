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

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<facebook::IGraphDelegate>> handler;
} // namespace

template <>
std::shared_ptr<facebook::IGraphDelegate> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IGraphDelegate> input) {
    handler->setValue(value, input);
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::IGraphDelegate>& input) {
    handler->setValue(value, input);
}

template <>
bool jsb_finalize<facebook::IGraphDelegate>(se::State& s) {
    return handler->finalize(s);
}
} // namespace core

namespace facebook {
namespace {
const auto jsb_GraphDelegate_finalize = &core::jsb_finalize<IGraphDelegate>;
const auto jsb_GraphDelegate_onSuccess =
    &core::jsb_set_callback<IGraphDelegate, &IGraphDelegate::onSuccess,
                            const std::string&>;
const auto jsb_GraphDelegate_onFailure =
    &core::jsb_set_callback<IGraphDelegate, &IGraphDelegate::onFailure,
                            const std::string&>;
const auto jsb_GraphDelegate_onCancel =
    &core::jsb_set_callback<IGraphDelegate, &IGraphDelegate::onCancel>;
} // namespace

SE_BIND_FINALIZE_FUNC(jsb_GraphDelegate_finalize);
SE_BIND_FUNC(jsb_GraphDelegate_onSuccess);
SE_BIND_FUNC(jsb_GraphDelegate_onFailure);
SE_BIND_FUNC(jsb_GraphDelegate_onCancel);

bool register_ifacebook_graph_delegate_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls =
        se::Class::create("IGraphDelegate", facebookObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_GraphDelegate_finalize));
    cls->defineFunction("onSuccess", _SE(jsb_GraphDelegate_onSuccess));
    cls->defineFunction("onFailure", _SE(jsb_GraphDelegate_onFailure));
    cls->defineFunction("onCancel", _SE(jsb_GraphDelegate_onCancel));

    cls->install();
    JSBClassType::registerClass<GraphDelegate>(cls);
    core::handler = core::SharedPtrHandler<IGraphDelegate>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
