//
//  jsb_facebook_graph_request.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/facebook/private/JsbFacebookGraphRequest.hpp"

#include <ee/core/JsbUtils.hpp>

#include "ee/facebook/FacebookGraphRequest.hpp"

using Self = ee::facebook::GraphRequest;

namespace ee {
namespace core {
template <>
facebook::GraphRequest get_value(const se::Value& value) {
    return *static_cast<Self*>(value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, Self& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace facebook {
namespace {
se::Class* clazz = nullptr;

// clang-format off
constexpr auto constructor  = &core::makeConstructor<Self>;
constexpr auto finalize     = &core::makeFinalize<Self>;
constexpr auto setPath      = &core::makeMethod<&Self::setPath>;
constexpr auto setParameter = &core::makeMethod<&Self::setParameter>;
constexpr auto toString     = &core::makeMethod<&Self::toString>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize);
SE_BIND_FUNC(setPath);
SE_BIND_FUNC(setParameter);
SE_BIND_FUNC(toString);
} // namespace

bool registerJsbGraphRequest(se::Object* global) {
    auto scope = JsbUtils::getPath(global, "ee");
    auto cls = se::Class::create("FacebookGraphRequest", scope, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));
    cls->defineFunction("setPath", _SE(setPath));
    cls->defineFunction("setParameter", _SE(setParameter));
    cls->defineFunction("toString", _SE(toString));

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
