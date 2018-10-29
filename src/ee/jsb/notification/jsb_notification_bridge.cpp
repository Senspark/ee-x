//
//  jsb_notification_bridge.cpp
//  ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//
#include "ee/jsb/notification/jsb_notification_bridge.hpp"

#include "ee/Notification.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace notification {

se::Class* __jsb_Notification_class = nullptr;

const auto jsb_Notification_finalize = &core::jsb_finalize<Notification>;
const auto jsb_Notification_constructor = &core::jsb_constructor<Notification>;
const auto jsb_Notification_schedule =
    &core::jsb_method_call<Notification, &Notification::schedule,
                           NotificationBuilder>;
const auto jsb_Notification_unschedule =
    &core::jsb_method_call<Notification, &Notification::unschedule, int>;
const auto jsb_Notification_clearAll =
    &core::jsb_method_call<Notification, &Notification::clearAll>;

SE_BIND_FINALIZE_FUNC(jsb_Notification_finalize)
SE_BIND_CTOR(jsb_Notification_constructor, __jsb_Notification_class,
             jsb_Notification_finalize)
SE_BIND_FUNC(jsb_Notification_schedule)
SE_BIND_FUNC(jsb_Notification_unschedule)
SE_BIND_FUNC(jsb_Notification_clearAll)

bool register_notification_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* notificationObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("notification", eeObj, &notificationObj);

    auto cls = se::Class::create("Notification", notificationObj, nullptr,
                                 _SE(jsb_Notification_constructor));
    cls->defineFinalizeFunction(_SE(jsb_Notification_finalize));

    cls->defineFunction("schedule", _SE(jsb_Notification_schedule));
    cls->defineFunction("unschedule", _SE(jsb_Notification_unschedule));
    cls->defineFunction("clearAll", _SE(jsb_Notification_clearAll));

    cls->install();

    JSBClassType::registerClass<Notification>(cls);

    __jsb_Notification_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace notification
} // namespace ee
