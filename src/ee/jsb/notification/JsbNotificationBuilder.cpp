//
//  jsb_notification_builder.cpp
//  ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//

#include "ee/jsb/notification/JsbNotificationBuilder.hpp"

#include <ee/Notification.hpp>
#include <ee/jsb/JsbCore.hpp>

using Self = ee::NotificationBuilder;

namespace ee {
namespace core {
template <>
Self get_value(const se::Value& value) {
    return *static_cast<Self*>(value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, Self& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace notification {
namespace {
se::Class* clazz = nullptr;

// clang-format off
constexpr auto constructor = &core::makeConstructor<Self>;
constexpr auto finalize    = &core::makeFinalize<Self>;
constexpr auto setBody     = &core::makeInstanceMethod<&Self::setBody>;
constexpr auto setDelay    = &core::makeInstanceMethod<&Self::setDelay>;
constexpr auto setInterval = &core::makeInstanceMethod<&Self::setInterval>;
constexpr auto setTag      = &core::makeInstanceMethod<&Self::setTag>;
constexpr auto setTicker   = &core::makeInstanceMethod<&Self::setTicker>;
constexpr auto setTitle    = &core::makeInstanceMethod<&Self::setTitle>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(setBody)
SE_BIND_FUNC(setDelay)
SE_BIND_FUNC(setInterval)
SE_BIND_FUNC(setTag)
SE_BIND_FUNC(setTicker)
SE_BIND_FUNC(setTitle)
} // namespace

bool registerJsbBuilder(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = se::Class::create("NotificationBuilder", scope, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setBody);
    EE_JSB_DEFINE_FUNCTION(cls, setDelay);
    EE_JSB_DEFINE_FUNCTION(cls, setInterval);
    EE_JSB_DEFINE_FUNCTION(cls, setTag);
    EE_JSB_DEFINE_FUNCTION(cls, setTicker);
    EE_JSB_DEFINE_FUNCTION(cls, setTitle);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace notification
} // namespace ee
