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

namespace ee {
namespace core {
template <>
facebook::RequestContent::ActionType get_value(const se::Value& value) {
    return static_cast<facebook::RequestContent::ActionType>(value.toInt32());
}

template <>
facebook::RequestContent::Filter get_value(const se::Value& value) {
    return static_cast<facebook::RequestContent::Filter>(value.toInt32());
}

template <>
facebook::RequestContent get_value(const se::Value& value) {
    return *static_cast<facebook::RequestContent*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, facebook::RequestContent& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace facebook {
namespace {
se::Class* clazz = nullptr;

using Self = RequestContent;

// clang-format off
constexpr auto constructor   = &core::makeConstructor<Self>;
constexpr auto finalize      = &core::makeFinalize<Self>;
constexpr auto setActionType = &core::makeInstanceMethod<&Self::setActionType>;
constexpr auto setFilter     = &core::makeInstanceMethod<&Self::setFilter>;
constexpr auto setRecipients = &core::makeInstanceMethod<&Self::setRecipients>;
constexpr auto setObjectId   = &core::makeInstanceMethod<&Self::setObjectId>;
constexpr auto setTitle      = &core::makeInstanceMethod<&Self::setTitle>;
constexpr auto setMessage    = &core::makeInstanceMethod<&Self::setMessage>;
constexpr auto setData       = &core::makeInstanceMethod<&Self::setData>;
constexpr auto toString      = &core::makeInstanceMethod<&Self::toString>;
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

    actionTypeObj->setProperty("None", se::Value(static_cast<std::int32_t>(
                                           RequestContent::ActionType::None)));
    actionTypeObj->setProperty("Send", se::Value(static_cast<std::int32_t>(
                                           RequestContent::ActionType::Send)));
    actionTypeObj->setProperty("AskFor",
                               se::Value(static_cast<std::int32_t>(
                                   RequestContent::ActionType::AskFor)));
    actionTypeObj->setProperty("Turn", se::Value(static_cast<std::int32_t>(
                                           RequestContent::ActionType::Turn)));

    filterObj->setProperty("None", se::Value(static_cast<std::int32_t>(
                                       RequestContent::Filter::None)));
    filterObj->setProperty("AppUsers", se::Value(static_cast<std::int32_t>(
                                           RequestContent::Filter::AppUsers)));
    filterObj->setProperty("AppNonUsers",
                           se::Value(static_cast<std::int32_t>(
                               RequestContent::Filter::AppNonUsers)));

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

    JSBClassType::registerClass<RequestContent>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
