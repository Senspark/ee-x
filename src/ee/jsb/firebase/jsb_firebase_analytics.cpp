//
// jsb_firebase_analytics.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/22/18
//
//

#include "ee/jsb/firebase/jsb_firebase_analytics.hpp"

#include "ee/Firebase.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace firebase {
namespace {
se::Class* clazz = nullptr;

using Self = FirebaseAnalytics;

// clang-format off
constexpr auto finalize                   = &core::makeFinalize<Self>;
constexpr auto constructor                = &core::makeConstructor<Self>;
constexpr auto initialize                 = &core::makeMethod<&Self::initialize>;
constexpr auto analyticsCollectionEnabled = &core::makeMethod<&Self::analyticsCollectionEnabled>;
constexpr auto setMinimumSessionDuration  = &core::makeMethod<&Self::setMinimumSessionDuration>;
constexpr auto setSessionTimeoutDuration  = &core::makeMethod<&Self::setSessionTimeoutDuration>;
constexpr auto setUserId                  = &core::makeMethod<&Self::setUserId>;
constexpr auto logEvent                   = &core::makeMethod<&Self::logEvent>;
// clang-format on

SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(initialize)
SE_BIND_FUNC(analyticsCollectionEnabled)
SE_BIND_FUNC(setMinimumSessionDuration)
SE_BIND_FUNC(setSessionTimeoutDuration)
SE_BIND_FUNC(setUserId)
SE_BIND_FUNC(logEvent)
} // namespace

bool register_firebase_analytics_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* firebaseObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("firebase", eeObj, &firebaseObj);

    auto cls = se::Class::create("FirebaseAnalytics", firebaseObj, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, initialize);
    EE_JSB_DEFINE_FUNCTION(cls, analyticsCollectionEnabled);
    EE_JSB_DEFINE_FUNCTION(cls, setMinimumSessionDuration);
    EE_JSB_DEFINE_FUNCTION(cls, setSessionTimeoutDuration);
    EE_JSB_DEFINE_FUNCTION(cls, setUserId);
    EE_JSB_DEFINE_FUNCTION(cls, logEvent);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace firebase
} // namespace ee
