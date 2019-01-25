//
//  jsb_google_analytics_tracker.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 8/23/18.
//

#include "ee/jsb/google/jsb_google_analytics_tracker.hpp"

#include "ee/GoogleAnalytics.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<google::AnalyticsTracker>> handler;
} // namespace

template <>
std::shared_ptr<google::AnalyticsTracker> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<google::AnalyticsTracker>& input) {
    handler->setValue(value, input);
}

template <>
bool makeFinalize<std::shared_ptr<google::AnalyticsTracker>>(se::State& state) {
    return handler->finalize(state);
}
} // namespace core

namespace google {
const auto jsb_AnalyticsTracker_finalize =
    &core::jsb_finalize<AnalyticsTracker>;
const auto jsb_AnalyticsTracker_setParameter =
    &core::jsb_method_call<AnalyticsTracker, &AnalyticsTracker::setParameter,
                           const std::string&, const std::string&>;
const auto jsb_AnalyticsTracker_setAllowIDFACollection =
    &core::jsb_method_call<AnalyticsTracker,
                           &AnalyticsTracker::setAllowIDFACollection, bool>;
const auto jsb_AnalyticsTracker_setScreenName =
    &core::jsb_method_call<AnalyticsTracker, &AnalyticsTracker::setScreenName,
                           const std::string&>;
const auto jsb_AnalyticsTracker_send =
    &core::jsb_method_call<AnalyticsTracker, &AnalyticsTracker::send,
                           std::map<std::string, std::string>>;

SE_BIND_FINALIZE_FUNC(jsb_AnalyticsTracker_finalize)
SE_BIND_FUNC(jsb_AnalyticsTracker_setParameter)
SE_BIND_FUNC(jsb_AnalyticsTracker_setAllowIDFACollection)
SE_BIND_FUNC(jsb_AnalyticsTracker_setScreenName)
SE_BIND_FUNC(jsb_AnalyticsTracker_send)

bool register_google_tracker_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* googleObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("google", eeObj, &googleObj);

    auto cls =
        se::Class::create("AnalyticsTracker", googleObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_AnalyticsTracker_finalize));

    cls->defineFunction("setParameter", _SE(jsb_AnalyticsTracker_setParameter));
    cls->defineFunction("setAllowIDFACollection",
                        _SE(jsb_AnalyticsTracker_setAllowIDFACollection));
    cls->defineFunction("setScreenName",
                        _SE(jsb_AnalyticsTracker_setScreenName));
    cls->defineFunction("send", _SE(jsb_AnalyticsTracker_send));

    cls->install();

    JSBClassType::registerClass<google::AnalyticsTracker>(cls);
    core::handler = core::SharedPtrHandler<google::AnalyticsTracker>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace google
} // namespace ee
