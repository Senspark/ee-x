//
// jsb_firebase_analytics.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/22/18
//
//

#include "ee/Firebase.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_core_common.hpp"

namespace ee {
namespace core {} // namespace core

namespace firebase {

static se::Object* __jsb_FirebaseAnalytics_proto = nullptr;
static se::Class* __jsb_FirebaseAnalytics_class = nullptr;

static se::Object* __firebaseObj = nullptr;

const auto jsb_FirebaseAnalytics_finalize =
    &core::jsb_finalize<FirebaseAnalytics>;
const auto jsb_FirebaseAnalytics_constructor =
    &core::jsb_constructor<FirebaseAnalytics>;
const auto jsb_FirebaseAnalytics_initialize =
    &core::jsb_method_get<FirebaseAnalytics, &FirebaseAnalytics::initialize,
                          bool>;
const auto jsb_FirebaseAnalytics_analyticsCollectionEnabled =
    &core::jsb_method_call<FirebaseAnalytics,
                           &FirebaseAnalytics::analyticsCollectionEnabled,
                           bool>;
const auto jsb_FirebaseAnalytics_setMinimumSessionDuration =
    &core::jsb_method_call<FirebaseAnalytics,
                           &FirebaseAnalytics::setMinimumSessionDuration,
                           std::int64_t>;
const auto jsb_FirebaseAnalytics_setSessionTimeoutDuration =
    &core::jsb_method_call<FirebaseAnalytics,
                           &FirebaseAnalytics::setSessionTimeoutDuration,
                           std::int64_t>;
const auto jsb_FirebaseAnalytics_setUserId =
    &core::jsb_method_call<FirebaseAnalytics, &FirebaseAnalytics::setUserId,
                           const std::string&>;
const auto jsb_FirebaseAnalytics_logEvent =
    &core::jsb_method_call<FirebaseAnalytics, &FirebaseAnalytics::logEvent,
                           const std::string&>;
const auto jsb_FirebaseAnalytics_logScreen =
    &core::jsb_method_call<FirebaseAnalytics, &FirebaseAnalytics::logScreen,
                           const std::string&>;
const auto jsb_FirebaseAnalytics_logCustomEvent = &core::jsb_method_call<
    FirebaseAnalytics, &FirebaseAnalytics::logCustomEvent, const std::string&,
    const std::string&, const std::string&, float>;
const auto jsb_FirebaseAnalytics_logEcommerce =
    &core::jsb_method_call<FirebaseAnalytics, &FirebaseAnalytics::logEcommerce,
                           const std::string&, const std::string&,
                           const std::string&, float, bool>;
const auto jsb_FirebaseAnalytics_logGameStart =
    &core::jsb_method_call<FirebaseAnalytics, &FirebaseAnalytics::logGameStart,
                           const std::string&, const std::string&>;
const auto jsb_FirebaseAnalytics_logGameEnd =
    &core::jsb_method_call<FirebaseAnalytics, &FirebaseAnalytics::logGameEnd,
                           const std::string&, const std::string&, int>;

SE_BIND_FINALIZE_FUNC(jsb_FirebaseAnalytics_finalize)
SE_BIND_CTOR(jsb_FirebaseAnalytics_constructor, __jsb_FirebaseAnalytics_class,
             jsb_FirebaseAnalytics_finalize)
SE_BIND_FUNC(jsb_FirebaseAnalytics_initialize)
SE_BIND_FUNC(jsb_FirebaseAnalytics_analyticsCollectionEnabled)
SE_BIND_FUNC(jsb_FirebaseAnalytics_setMinimumSessionDuration)
SE_BIND_FUNC(jsb_FirebaseAnalytics_setSessionTimeoutDuration)
SE_BIND_FUNC(jsb_FirebaseAnalytics_setUserId)
SE_BIND_FUNC(jsb_FirebaseAnalytics_logEvent)
SE_BIND_FUNC(jsb_FirebaseAnalytics_logScreen)
SE_BIND_FUNC(jsb_FirebaseAnalytics_logCustomEvent)
SE_BIND_FUNC(jsb_FirebaseAnalytics_logEcommerce)
SE_BIND_FUNC(jsb_FirebaseAnalytics_logGameStart)
SE_BIND_FUNC(jsb_FirebaseAnalytics_logGameEnd)

bool register_firebase_manual(se::Object* globalObj) {
    core::getOrCreatePlainObject_r("firebase", core::__eeObj, &__firebaseObj);

    auto cls = se::Class::create("FirebaseAnalytics", __firebaseObj, nullptr,
                                 _SE(jsb_FirebaseAnalytics_constructor));
    cls->defineFinalizeFunction(_SE(jsb_FirebaseAnalytics_finalize));

    cls->defineFunction("initialize", _SE(jsb_FirebaseAnalytics_initialize));
    cls->defineFunction("analyticsCollectionEnabled",
                        _SE(jsb_FirebaseAnalytics_analyticsCollectionEnabled));
    cls->defineFunction("setMinimumSessionDuration",
                        _SE(jsb_FirebaseAnalytics_setMinimumSessionDuration));
    cls->defineFunction("setSessionTimeoutDuration",
                        _SE(jsb_FirebaseAnalytics_setSessionTimeoutDuration));
    cls->defineFunction("setUserId", _SE(jsb_FirebaseAnalytics_setUserId));
    cls->defineFunction("logEvent", _SE(jsb_FirebaseAnalytics_logEvent));
    cls->defineFunction("logScreen", _SE(jsb_FirebaseAnalytics_logScreen));
    cls->defineFunction("logCustomEvent",
                        _SE(jsb_FirebaseAnalytics_logCustomEvent));
    cls->defineFunction("logEcommerce",
                        _SE(jsb_FirebaseAnalytics_logEcommerce));
    cls->defineFunction("logGameStart",
                        _SE(jsb_FirebaseAnalytics_logGameStart));
    cls->defineFunction("logGameEnd", _SE(jsb_FirebaseAnalytics_logGameEnd));

    cls->install();

    JSBClassType::registerClass<ee::FirebaseAnalytics>(cls);

    __jsb_FirebaseAnalytics_proto = cls->getProto();
    __jsb_FirebaseAnalytics_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace firebase
} // namespace ee
