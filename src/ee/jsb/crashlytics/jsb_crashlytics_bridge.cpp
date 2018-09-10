#include "ee/Crashlytics.hpp"
#include "ee/jsb/core/jsb_log_level.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_core_common.hpp"

namespace ee {
namespace core {} // namespace core

namespace crashlytics {

static se::Object* __jsb_Crashlytics_proto = nullptr;
static se::Class* __jsb_Crashlytics_class = nullptr;

se::Object* __crashlyticsObj = nullptr;

const auto jsb_Crashlytics_finalize = &core::jsb_finalize<Crashlytics>;
const auto jsb_Crashlytics_constructor = &core::jsb_constructor<Crashlytics>;
const auto jsb_Crashlytics_causeCrash =
    &core::jsb_method_call<Crashlytics, &Crashlytics::causeCrash>;
const auto jsb_Crashlytics_causeException =
    &core::jsb_method_call<Crashlytics, &Crashlytics::causeException>;
const auto jsb_Crashlytics_setLogLevel =
    &core::jsb_method_call<Crashlytics, &Crashlytics::setLogLevel,
                           const LogLevel&>;
const auto jsb_Crashlytics_log =
    &core::jsb_method_call<Crashlytics, &Crashlytics::log, const LogLevel&,
                           const std::string&, const std::string&>;
const auto jsb_Crashlytics_setString =
    &core::jsb_method_call<Crashlytics, &Crashlytics::setString,
                           const std::string&, const std::string&>;
const auto jsb_Crashlytics_setBool =
    &core::jsb_method_call<Crashlytics, &Crashlytics::setBool,
                           const std::string&, bool>;
const auto jsb_Crashlytics_setInt32 =
    &core::jsb_method_call<Crashlytics, &Crashlytics::setInt,
                           const std::string&, std::int32_t>;
const auto jsb_Crashlytics_setUserIdentifier =
    &core::jsb_method_call<Crashlytics, &Crashlytics::setUserIdentifier,
                           const std::string&>;
const auto jsb_Crashlytics_setUserName =
    &core::jsb_method_call<Crashlytics, &Crashlytics::setUserName,
                           const std::string&>;
const auto jsb_Crashlytics_setUserEmail =
    &core::jsb_method_call<Crashlytics, &Crashlytics::setUserEmail,
                           const std::string&>;
const auto jsb_Crashlytics_trackLevelStart =
    &core::jsb_method_call<Crashlytics, &Crashlytics::trackLevelStart,
                           const std::string&,
                           std::unordered_map<std::string, std::string>>;
const auto jsb_Crashlytics_trackLevelEnd =
    &core::jsb_method_call<Crashlytics, &Crashlytics::trackLevelEnd,
                           const std::string&, int32_t, bool,
                           std::unordered_map<std::string, std::string>>;
const auto jsb_Crashlytics_trackPurchase =
    &core::jsb_method_call<Crashlytics, &Crashlytics::trackPurchase, float,
                           const std::string&, bool, const std::string&,
                           const std::string&, const std::string&,
                           std::unordered_map<std::string, std::string>>;
const auto jsb_Crashlytics_trackCustomEvent =
    &core::jsb_method_call<Crashlytics, &Crashlytics::trackCustomEvent,
                           const std::string&,
                           std::unordered_map<std::string, std::string>>;
const auto jsb_Crashlytics_trackInvite =
    &core::jsb_method_call<Crashlytics, &Crashlytics::trackInvite,
                           const std::string&,
                           std::unordered_map<std::string, std::string>>;

SE_BIND_FINALIZE_FUNC(jsb_Crashlytics_finalize)
SE_BIND_CTOR(jsb_Crashlytics_constructor, __jsb_Crashlytics_class,
             jsb_Crashlytics_finalize)
SE_BIND_FUNC(jsb_Crashlytics_causeCrash)
SE_BIND_FUNC(jsb_Crashlytics_causeException)
SE_BIND_FUNC(jsb_Crashlytics_setLogLevel)
SE_BIND_FUNC(jsb_Crashlytics_log)
SE_BIND_FUNC(jsb_Crashlytics_setString)
SE_BIND_FUNC(jsb_Crashlytics_setBool)
SE_BIND_FUNC(jsb_Crashlytics_setInt32)
SE_BIND_FUNC(jsb_Crashlytics_setUserIdentifier)
SE_BIND_FUNC(jsb_Crashlytics_setUserName)
SE_BIND_FUNC(jsb_Crashlytics_setUserEmail)
SE_BIND_FUNC(jsb_Crashlytics_trackLevelStart)
SE_BIND_FUNC(jsb_Crashlytics_trackLevelEnd)
SE_BIND_FUNC(jsb_Crashlytics_trackPurchase)
SE_BIND_FUNC(jsb_Crashlytics_trackCustomEvent)
SE_BIND_FUNC(jsb_Crashlytics_trackInvite)

bool register_crashlytics_bridge_manual(se::Object* globalObj) {
    core::getOrCreatePlainObject_r("crashlytics", core::__eeObj,
                                   &__crashlyticsObj);

    auto cls = se::Class::create("Crashlytics", __crashlyticsObj, nullptr,
                                 _SE(jsb_Crashlytics_constructor));
    cls->defineFinalizeFunction(_SE(jsb_Crashlytics_finalize));

    cls->defineFunction("causeCrash", _SE(jsb_Crashlytics_causeCrash));
    cls->defineFunction("causeException", _SE(jsb_Crashlytics_causeException));
    cls->defineFunction("setLogLevel", _SE(jsb_Crashlytics_setLogLevel));
    cls->defineFunction("log", _SE(jsb_Crashlytics_log));
    cls->defineFunction("setString", _SE(jsb_Crashlytics_setString));
    cls->defineFunction("setBool", _SE(jsb_Crashlytics_setBool));
    cls->defineFunction("setInt32", _SE(jsb_Crashlytics_setInt32));
    cls->defineFunction("setUserIdentifier",
                        _SE(jsb_Crashlytics_setUserIdentifier));
    cls->defineFunction("setUserName", _SE(jsb_Crashlytics_setUserName));
    cls->defineFunction("setUserEmail", _SE(jsb_Crashlytics_setUserEmail));
    cls->defineFunction("trackLevelStart",
                        _SE(jsb_Crashlytics_trackLevelStart));
    cls->defineFunction("trackLevelEnd", _SE(jsb_Crashlytics_trackLevelEnd));
    cls->defineFunction("trackPurchase", _SE(jsb_Crashlytics_trackPurchase));
    cls->defineFunction("trackCustomEvent",
                        _SE(jsb_Crashlytics_trackCustomEvent));
    cls->defineFunction("trackInvite", _SE(jsb_Crashlytics_trackInvite));

    cls->install();

    JSBClassType::registerClass<ee::Crashlytics>(cls);

    __jsb_Crashlytics_proto = cls->getProto();
    __jsb_Crashlytics_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace crashlytics
} // namespace ee
