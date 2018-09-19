//
//  jsb_google_analytics_tracker.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 8/23/18.
//

#include "ee/GoogleAnalytics.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_core_common.hpp"

namespace ee {

namespace google {
static std::vector<std::shared_ptr<ee::GoogleAnalyticsTracker>>
    __jsb_s_googleAnalyticsArchive;
static se::Object* __jsb_AnalyticsTracker_proto = nullptr;
static se::Class* __jsb_AnalyticsTracker_class = nullptr;
} // namespace google

namespace core {
template <>
void set_value(se::Value& value,
               std::shared_ptr<ee::GoogleAnalyticsTracker> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        google::__jsb_s_googleAnalyticsArchive.push_back(input);
        obj = se::Object::createObjectWithClass(
            google::__jsb_AnalyticsTracker_class);
        obj->setPrivateData(input.get());
        value.setObject(obj);
    } else {
        value.setNull();
    }
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
    se::Object* googleObj = nullptr;
    core::getOrCreatePlainObject_r("google", core::__eeObj, &googleObj);

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

    JSBClassType::registerClass<ee::google::AnalyticsTracker>(cls);

    __jsb_AnalyticsTracker_proto = cls->getProto();
    __jsb_AnalyticsTracker_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace google
} // namespace ee
