//
//  jsb_facebook_request_content.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/facebook/jsb_facebook_request_content.hpp"

#include "ee/facebook/FacebookRequestContent.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

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

bool register_facebook_request_content_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    se::Object* reqContentObj = nullptr;
    se::Object* actionTypeObj = nullptr;
    se::Object* filterObj = nullptr;

    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);
    ee::core::getOrCreatePlainObject_r("RequestContent", facebookObj,
                                       &reqContentObj);
    ee::core::getOrCreatePlainObject_r("ActionType", reqContentObj,
                                       &actionTypeObj);
    ee::core::getOrCreatePlainObject_r("Filter", reqContentObj, &filterObj);

    actionTypeObj->setProperty(
        "None", //
        se::Value(static_cast<std::int32_t>(Self::ActionType::None)));
    actionTypeObj->setProperty(
        "Send", //
        se::Value(static_cast<std::int32_t>(Self::ActionType::Send)));
    actionTypeObj->setProperty(
        "AskFor", //
        se::Value(static_cast<std::int32_t>(Self::ActionType::AskFor)));
    actionTypeObj->setProperty(
        "Turn", //
        se::Value(static_cast<std::int32_t>(Self::ActionType::Turn)));

    filterObj->setProperty(
        "None", //
        se::Value(static_cast<std::int32_t>(Self::Filter::None)));
    filterObj->setProperty(
        "AppUsers", //
        se::Value(static_cast<std::int32_t>(Self::Filter::AppUsers)));
    filterObj->setProperty(
        "AppNonUsers", //
        se::Value(static_cast<std::int32_t>(Self::Filter::AppNonUsers)));

    auto cls = se::Class::create("RequestContent", facebookObj, nullptr,
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
