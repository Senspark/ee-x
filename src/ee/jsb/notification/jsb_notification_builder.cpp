//
//  jsb_notification_builder.cpp
//  ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//

#include "ee/jsb/notification/jsb_notification_builder.hpp"

#include "ee/Notification.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee
{
namespace core
{
template <>
NotificationBuilder get_value(const se::Value &value)
{
    return *static_cast<NotificationBuilder *>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value &value, NotificationBuilder &input)
{
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace notification
{

se::Class *__jsb_NotificationBuilder_class = nullptr;

const auto jsb_NotificationBuilder_finalize =
    &core::jsb_finalize<NotificationBuilder>;
const auto jsb_NotificationBuilder_constructor =
    &core::jsb_constructor<NotificationBuilder>;
const auto jsb_NotificationBuilder_setTicker = &core::jsb_accessor_set<
    NotificationBuilder, &NotificationBuilder::setTicker, const std::string &>;
const auto jsb_NotificationBuilder_setTitle =
    &core::jsb_accessor_set<NotificationBuilder, &NotificationBuilder::setTitle,
                            const std::string &>;
const auto jsb_NotificationBuilder_setBody =
    &core::jsb_accessor_set<NotificationBuilder, &NotificationBuilder::setBody,
                            const std::string &>;
const auto jsb_NotificationBuilder_setInterval =
    &core::jsb_accessor_set<NotificationBuilder,
                            &NotificationBuilder::setInterval, int>;
const auto jsb_NotificationBuilder_setDelay =
    &core::jsb_accessor_set<NotificationBuilder,
                            &NotificationBuilder::setDelay, int>;
const auto jsb_NotificationBuilder_setTag =
    &core::jsb_accessor_set<NotificationBuilder, &NotificationBuilder::setTag,
                            int>;

SE_BIND_FINALIZE_FUNC(jsb_NotificationBuilder_finalize)
SE_BIND_CTOR(jsb_NotificationBuilder_constructor,
             __jsb_NotificationBuilder_class, jsb_NotificationBuilder_finalize)
SE_BIND_FUNC(jsb_NotificationBuilder_setTicker)
SE_BIND_FUNC(jsb_NotificationBuilder_setTitle)
SE_BIND_FUNC(jsb_NotificationBuilder_setDelay)
SE_BIND_FUNC(jsb_NotificationBuilder_setBody)
SE_BIND_FUNC(jsb_NotificationBuilder_setInterval)
SE_BIND_FUNC(jsb_NotificationBuilder_setTag)

bool register_notification_builder_manual(se::Object *globalObj)
{
    se::Object *eeObj = nullptr;
    se::Object *notificationObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("notification", eeObj, &notificationObj);

    auto cls =
        se::Class::create("NotificationBuilder", notificationObj, nullptr,
                          _SE(jsb_NotificationBuilder_constructor));
    cls->defineFinalizeFunction(_SE(jsb_NotificationBuilder_finalize));

    cls->defineFunction("setTicker", _SE(jsb_NotificationBuilder_setTicker));
    cls->defineFunction("setTitle", _SE(jsb_NotificationBuilder_setTitle));
    cls->defineFunction("setDelay", _SE(jsb_NotificationBuilder_setDelay));
    cls->defineFunction("setBody", _SE(jsb_NotificationBuilder_setBody));
    cls->defineFunction("setInterval",
                        _SE(jsb_NotificationBuilder_setInterval));
    cls->defineFunction("setTag", _SE(jsb_NotificationBuilder_setTag));

    cls->install();

    JSBClassType::registerClass<NotificationBuilder>(cls);

    __jsb_NotificationBuilder_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace notification
} // namespace ee
