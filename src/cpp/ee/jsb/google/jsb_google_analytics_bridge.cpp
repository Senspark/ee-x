//
// jsb_google_analytics_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//
//

#include "ee/jsb/google/jsb_google_analytics_bridge.hpp"

#include <ee/GoogleAnalytics.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace google {

se::Class* __jsb_GoogleAnalytics_class = nullptr;

const auto jsb_GoogleAnalytics_finalize = &core::jsb_finalize<GoogleAnalytics>;
const auto jsb_GoogleAnalytics_constructor =
    &core::jsb_constructor<GoogleAnalytics>;
const auto jsb_GoogleAnalytics_setDispatchInterval =
    &core::jsb_method_call<GoogleAnalytics,
                           &GoogleAnalytics::setDispatchInterval, int>;
const auto jsb_GoogleAnalytics_setDryRun =
    &core::jsb_method_call<GoogleAnalytics, &GoogleAnalytics::setDryRun, bool>;
const auto jsb_GoogleAnalytics_setOptOut =
    &core::jsb_method_call<GoogleAnalytics, &GoogleAnalytics::setOptOut, bool>;
const auto jsb_GoogleAnalytics_setTrackUncaughtException =
    &core::jsb_method_call<GoogleAnalytics,
                           &GoogleAnalytics::setTrackUncaughtException, bool>;
const auto jsb_GoogleAnalytics_dispatch =
    &core::jsb_method_call<GoogleAnalytics, &GoogleAnalytics::dispatch>;
const auto jsb_GoogleAnalytics_createTracker =
    &core::jsb_method_get<GoogleAnalytics, &GoogleAnalytics::createTracker,
                          const std::shared_ptr<AnalyticsTracker>&,
                          const std::string&>;
const auto jsb_GoogleAnalytics_doTests =
    &core::jsb_method_call<GoogleAnalytics, &GoogleAnalytics::doTests>;

SE_BIND_FINALIZE_FUNC(jsb_GoogleAnalytics_finalize)
SE_BIND_CTOR(jsb_GoogleAnalytics_constructor, __jsb_GoogleAnalytics_class,
             jsb_GoogleAnalytics_finalize)
SE_BIND_FUNC(jsb_GoogleAnalytics_setDispatchInterval)
SE_BIND_FUNC(jsb_GoogleAnalytics_setDryRun)
SE_BIND_FUNC(jsb_GoogleAnalytics_setOptOut)
SE_BIND_FUNC(jsb_GoogleAnalytics_setTrackUncaughtException)
SE_BIND_FUNC(jsb_GoogleAnalytics_dispatch)
SE_BIND_FUNC(jsb_GoogleAnalytics_createTracker)
SE_BIND_FUNC(jsb_GoogleAnalytics_doTests)

bool register_google_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* googleObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("google", eeObj, &googleObj);

    auto cls = se::Class::create("GoogleAnalytics", googleObj, nullptr,
                                 _SE(jsb_GoogleAnalytics_constructor));
    cls->defineFinalizeFunction(_SE(jsb_GoogleAnalytics_finalize));

    cls->defineFunction("setDispatchInterval",
                        _SE(jsb_GoogleAnalytics_setDispatchInterval));
    cls->defineFunction("setDispatchInterval",
                        _SE(jsb_GoogleAnalytics_setDispatchInterval));
    cls->defineFunction("setDryRun", _SE(jsb_GoogleAnalytics_setDryRun));
    cls->defineFunction("setOptOut", _SE(jsb_GoogleAnalytics_setOptOut));
    cls->defineFunction("setTrackUncaughtException",
                        _SE(jsb_GoogleAnalytics_setTrackUncaughtException));
    cls->defineFunction("dispatch", _SE(jsb_GoogleAnalytics_dispatch));
    cls->defineFunction("createTracker",
                        _SE(jsb_GoogleAnalytics_createTracker));
    cls->defineFunction("doTests", _SE(jsb_GoogleAnalytics_doTests));

    cls->install();

    JSBClassType::registerClass<GoogleAnalytics>(cls);

    __jsb_GoogleAnalytics_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace google
} // namespace ee
