//
//  jsb_facebook_request_content.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/facebook/private/JsbFacebookRequestContent.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>

#include "ee/facebook/FacebookRequestContent.hpp"

using Self = ee::facebook::RequestContent;

namespace ee {
namespace core {
template <>
Self::ActionType get_value(const se::Value& value) {
    return static_cast<Self::ActionType>(value.toInt32());
}

template <>
Self::Filter get_value(const se::Value& value) {
    return static_cast<Self::Filter>(value.toInt32());
}

template <>
Self get_value(const se::Value& value) {
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
constexpr auto constructor   = &core::makeConstructor<Self>;
constexpr auto finalize      = &core::makeFinalize<Self>;
constexpr auto setActionType = &core::makeMethod<&Self::setActionType>;
constexpr auto setFilter     = &core::makeMethod<&Self::setFilter>;
constexpr auto setRecipients = &core::makeMethod<&Self::setRecipients>;
constexpr auto setObjectId   = &core::makeMethod<&Self::setObjectId>;
constexpr auto setTitle      = &core::makeMethod<&Self::setTitle>;
constexpr auto setMessage    = &core::makeMethod<&Self::setMessage>;
constexpr auto setData       = &core::makeMethod<&Self::setData>;
constexpr auto toString      = &core::makeMethod<&Self::toString>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize);
SE_BIND_FUNC(setActionType);
SE_BIND_FUNC(setFilter);
SE_BIND_FUNC(setRecipients);
SE_BIND_FUNC(setObjectId);
SE_BIND_FUNC(setTitle);
SE_BIND_FUNC(setMessage);
SE_BIND_FUNC(setData);
SE_BIND_FUNC(toString);
} // namespace

bool registerJsbRequestContent(se::Object* global) {
    auto scope = JsbUtils::getPath(global, "ee");

    // Register FacebookRequestContent::ActionType class.
    auto actionType = JsbUtils::getPath(scope, "FacebookRequestContent/ActionType");
    actionType->setProperty(
        "None", //
        se::Value(static_cast<std::int32_t>(Self::ActionType::None)));
    actionType->setProperty(
        "Send", //
        se::Value(static_cast<std::int32_t>(Self::ActionType::Send)));
    actionType->setProperty(
        "AskFor", //
        se::Value(static_cast<std::int32_t>(Self::ActionType::AskFor)));
    actionType->setProperty(
        "Turn", //
        se::Value(static_cast<std::int32_t>(Self::ActionType::Turn)));

    // Register FacebookRequestContent::Filter class.
    auto filter = JsbUtils::getPath(scope, "FacebookRequestContent/Filter");
    filter->setProperty(
        "None", //
        se::Value(static_cast<std::int32_t>(Self::Filter::None)));
    filter->setProperty(
        "AppUsers", //
        se::Value(static_cast<std::int32_t>(Self::Filter::AppUsers)));
    filter->setProperty(
        "AppNonUsers", //
        se::Value(static_cast<std::int32_t>(Self::Filter::AppNonUsers)));

    // Register FacebookRequestContent class.
    auto cls = se::Class::create("FacebookRequestContent", scope, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setActionType);
    EE_JSB_DEFINE_FUNCTION(cls, setFilter);
    EE_JSB_DEFINE_FUNCTION(cls, setRecipients);
    EE_JSB_DEFINE_FUNCTION(cls, setObjectId);
    EE_JSB_DEFINE_FUNCTION(cls, setTitle);
    EE_JSB_DEFINE_FUNCTION(cls, setMessage);
    EE_JSB_DEFINE_FUNCTION(cls, setData);
    EE_JSB_DEFINE_FUNCTION(cls, toString);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee

#endif // EE_X_COCOS_JS