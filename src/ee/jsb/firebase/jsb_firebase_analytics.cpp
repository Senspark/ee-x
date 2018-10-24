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

se::Object* __jsb_FirebaseAnalytics_proto = nullptr;
se::Class* __jsb_FirebaseAnalytics_class = nullptr;

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
                           const std::string&,
                           std::unordered_map<std::string, std::string>>;

SE_BIND_FINALIZE_FUNC(jsb_FirebaseAnalytics_finalize)
SE_BIND_CTOR(jsb_FirebaseAnalytics_constructor, __jsb_FirebaseAnalytics_class,
             jsb_FirebaseAnalytics_finalize)
SE_BIND_FUNC(jsb_FirebaseAnalytics_initialize)
SE_BIND_FUNC(jsb_FirebaseAnalytics_analyticsCollectionEnabled)
SE_BIND_FUNC(jsb_FirebaseAnalytics_setMinimumSessionDuration)
SE_BIND_FUNC(jsb_FirebaseAnalytics_setSessionTimeoutDuration)
SE_BIND_FUNC(jsb_FirebaseAnalytics_setUserId)
SE_BIND_FUNC(jsb_FirebaseAnalytics_logEvent)

bool register_firebase_analytics_manual(se::Object* globalObj) {
    se::Object* __firebaseObj = nullptr;
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

    cls->install();

    JSBClassType::registerClass<ee::FirebaseAnalytics>(cls);

    __jsb_FirebaseAnalytics_proto = cls->getProto();
    __jsb_FirebaseAnalytics_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace firebase
} // namespace ee
