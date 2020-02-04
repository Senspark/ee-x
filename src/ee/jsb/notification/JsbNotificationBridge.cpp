//
//  jsb_notification_bridge.cpp
//  ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//

#include "ee/jsb/notification/JsbNotificationBridge.hpp"

#include <ee/Notification.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace notification {
using Self = Bridge;

namespace {
se::Class *clazz = nullptr;

// clang-format off
constexpr auto constructor = &core::makeConstructor<Self>;
constexpr auto finalize    = &core::makeFinalize<Self>;
constexpr auto schedule    = &core::makeInstanceMethod<
                                 static_cast<void (Self::*)(const NotificationBuilder&)>(&Self::schedule),
                                 static_cast<void (Self::*)(const std::string&, int, int, int)>(&Self::schedule)>;
constexpr auto unschedule  = &core::makeInstanceMethod<&Self::unschedule>;
constexpr auto clearAll    = &core::makeInstanceMethod<&Self::clearAll>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(schedule)
SE_BIND_FUNC(unschedule)
SE_BIND_FUNC(clearAll)
} // namespace

bool registerJsbBridge(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls =
        se::Class::create("Notification", scope, nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, schedule);
    EE_JSB_DEFINE_FUNCTION(cls, unschedule);
    EE_JSB_DEFINE_FUNCTION(cls, clearAll);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace notification
} // namespace ee
