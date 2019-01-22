//
//  jsb_facebook_graph_request.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/facebook/jsb_facebook_graph_request.hpp"

#include "ee/facebook/FacebookGraphRequest.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
template <>
facebook::GraphRequest get_value(const se::Value& value) {
    return *static_cast<facebook::GraphRequest*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, facebook::GraphRequest& input) {
    value.toObject()->setPrivateData(&input);
}
} // namespace core

namespace facebook {
namespace {
se::Class* clazz = nullptr;

using Self = GraphRequest;

// clang-format off
constexpr auto constructor  = &core::makeConstructor<Self>;
constexpr auto finalize     = &core::makeFinalize<Self>;
constexpr auto setPath      = &core::makeInstanceMethod<&Self::setPath>;
constexpr auto setParameter = &core::makeInstanceMethod<&Self::setParameter>;
constexpr auto toString     = &core::makeInstanceMethod<&Self::toString>;
// clang-format on

SE_BIND_CTOR(constructor, clazz, finalize);
SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(setPath);
SE_BIND_FUNC(setParameter);
SE_BIND_FUNC(toString);
} // namespace

bool register_facebook_graph_request_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("GraphRequest", facebookObj, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));
    cls->defineFunction("setPath", _SE(setPath));
    cls->defineFunction("setParameter", _SE(setParameter));
    cls->defineFunction("toString", _SE(toString));

    cls->install();

    JSBClassType::registerClass<GraphRequest>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
