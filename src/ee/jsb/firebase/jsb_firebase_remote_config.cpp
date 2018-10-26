//
// jsb_firebase_remote_config.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 9/23/18
//
//

#include "ee/jsb/firebase/jsb_firebase_remote_config.hpp"

#include "ee/Firebase.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {

namespace firebase {

se::Class* __jsb_FirebaseRemoteConfig_class = nullptr;

const auto jsb_FirebaseRemoteConfig_finalize =
    &core::jsb_finalize<FirebaseRemoteConfig>;
const auto jsb_FirebaseRemoteConfig_constructor =
    &core::jsb_constructor<FirebaseRemoteConfig>;
const auto jsb_FirebaseRemoteConfig_initialize =
    &core::jsb_method_get<FirebaseRemoteConfig,
                          &FirebaseRemoteConfig::initialize, bool>;
const auto jsb_FirebaseRemoteConfig_fetchJS =
    &core::jsb_set_callback<FirebaseRemoteConfig,
                            &FirebaseRemoteConfig::fetchJS, bool>;
const auto jsb_FirebaseRemoteConfig_setDefaultBool =
    &core::jsb_method_call<FirebaseRemoteConfig,
                           &FirebaseRemoteConfig::setDefaultBool,
                           const std::string&, bool>;
const auto jsb_FirebaseRemoteConfig_setDefaultLong =
    &core::jsb_method_call<FirebaseRemoteConfig,
                           &FirebaseRemoteConfig::setDefaultLong,
                           const std::string&, std::int64_t>;
const auto jsb_FirebaseRemoteConfig_setDefaultDouble =
    &core::jsb_method_call<FirebaseRemoteConfig,
                           &FirebaseRemoteConfig::setDefaultDouble,
                           const std::string&, double>;
const auto jsb_FirebaseRemoteConfig_setDefaultString =
    &core::jsb_method_call<FirebaseRemoteConfig,
                           &FirebaseRemoteConfig::setDefaultString,
                           const std::string&, const std::string&>;
const auto jsb_FirebaseRemoteConfig_flushDefaults =
    &core::jsb_method_call<FirebaseRemoteConfig,
                           &FirebaseRemoteConfig::flushDefaults>;
const auto jsb_FirebaseRemoteConfig_getBool =
    &core::jsb_method_get<FirebaseRemoteConfig, &FirebaseRemoteConfig::getBool,
                          bool, const std::string&>;
const auto jsb_FirebaseRemoteConfig_getLong =
    &core::jsb_method_get<FirebaseRemoteConfig, &FirebaseRemoteConfig::getLong,
                          std::int64_t, const std::string&>;
const auto jsb_FirebaseRemoteConfig_getDouble =
    &core::jsb_method_get<FirebaseRemoteConfig,
                          &FirebaseRemoteConfig::getDouble, double,
                          const std::string&>;
const auto jsb_FirebaseRemoteConfig_getString =
    &core::jsb_method_get<FirebaseRemoteConfig,
                          &FirebaseRemoteConfig::getString, const std::string&,
                          const std::string&>;

SE_BIND_FINALIZE_FUNC(jsb_FirebaseRemoteConfig_finalize)
SE_BIND_CTOR(jsb_FirebaseRemoteConfig_constructor,
             __jsb_FirebaseRemoteConfig_class,
             jsb_FirebaseRemoteConfig_finalize)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_initialize)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_fetchJS)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_setDefaultBool)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_setDefaultLong)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_setDefaultDouble)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_setDefaultString)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_flushDefaults)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_getBool)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_getLong)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_getDouble)
SE_BIND_FUNC(jsb_FirebaseRemoteConfig_getString)

bool register_firebase_remote_config_manual(se::Object* globalObj) {
    se::Object* firebaseObj = nullptr;
    se::Object* eeObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("firebase", eeObj, &firebaseObj);

    auto cls = se::Class::create("FirebaseRemoteConfig", firebaseObj, nullptr,
                                 _SE(jsb_FirebaseRemoteConfig_constructor));
    cls->defineFinalizeFunction(_SE(jsb_FirebaseRemoteConfig_finalize));

    cls->defineFunction("initialize", _SE(jsb_FirebaseRemoteConfig_initialize));
    cls->defineFunction("fetchJS", _SE(jsb_FirebaseRemoteConfig_fetchJS));
    cls->defineFunction("setDefaultBool",
                        _SE(jsb_FirebaseRemoteConfig_setDefaultBool));
    cls->defineFunction("setDefaultLong",
                        _SE(jsb_FirebaseRemoteConfig_setDefaultLong));
    cls->defineFunction("setDefaultDouble",
                        _SE(jsb_FirebaseRemoteConfig_setDefaultDouble));
    cls->defineFunction("setDefaultString",
                        _SE(jsb_FirebaseRemoteConfig_setDefaultString));
    cls->defineFunction("flushDefaults",
                        _SE(jsb_FirebaseRemoteConfig_flushDefaults));
    cls->defineFunction("getBool", _SE(jsb_FirebaseRemoteConfig_getBool));
    cls->defineFunction("getLong", _SE(jsb_FirebaseRemoteConfig_getLong));
    cls->defineFunction("getDouble", _SE(jsb_FirebaseRemoteConfig_getDouble));
    cls->defineFunction("getString", _SE(jsb_FirebaseRemoteConfig_getString));

    cls->install();

    JSBClassType::registerClass<FirebaseRemoteConfig>(cls);

    __jsb_FirebaseRemoteConfig_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace firebase
} // namespace ee
