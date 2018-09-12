//
//  jsb_google_analytics_event_builder.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/5/18.
//

#include "ee/GoogleAnalytics.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_core_common.hpp"

namespace ee {

namespace core {
template <>
ee::GoogleEventBuilder get_value(const se::Value& value) {
    return *static_cast<ee::GoogleEventBuilder*>(
        value.toObject()->getPrivateData());
}
} // namespace core

namespace google {

static se::Object* __jsb_EventBuilder_proto = nullptr;
static se::Class* __jsb_EventBuilder_class = nullptr;

static se::Object* __googleObj = nullptr;

const auto jsb_EventBuilder_finalize =
    &core::jsb_finalize<ee::GoogleEventBuilder>;
const auto jsb_EventBuilder_constructor =
    &core::jsb_constructor<ee::GoogleEventBuilder, const std::string&,
                           const std::string&>;
const auto jsb_EventBuilder_setCategory =
    &core::jsb_accessor_set<ee::GoogleEventBuilder,
                            &ee::GoogleEventBuilder::setCategory,
                            const std::string&>;
const auto jsb_EventBuilder_setAction =
    &core::jsb_accessor_set<ee::GoogleEventBuilder,
                            &ee::GoogleEventBuilder::setAction,
                            const std::string&>;
const auto jsb_EventBuilder_setLabel =
    &core::jsb_accessor_set<ee::GoogleEventBuilder,
                            &ee::GoogleEventBuilder::setLabel,
                            const std::string&>;
const auto jsb_EventBuilder_setValue =
    &core::jsb_accessor_set<ee::GoogleEventBuilder,
                            &ee::GoogleEventBuilder::setValue, int>;
const auto jsb_EventBuilder_build =
    &core::jsb_method_get<ee::GoogleEventBuilder,
                          &ee::GoogleEventBuilder::build,
                          std::map<std::string, std::string>>;

SE_BIND_FINALIZE_FUNC(jsb_EventBuilder_finalize)
SE_BIND_CTOR(jsb_EventBuilder_constructor, __jsb_EventBuilder_class,
             jsb_EventBuilder_finalize)
SE_BIND_FUNC(jsb_EventBuilder_setCategory)
SE_BIND_FUNC(jsb_EventBuilder_setAction)
SE_BIND_FUNC(jsb_EventBuilder_setLabel)
SE_BIND_FUNC(jsb_EventBuilder_setValue)
SE_BIND_FUNC(jsb_EventBuilder_build)

bool register_google_event_builder_manual(se::Object* globalObj) {
    core::getOrCreatePlainObject_r("google", core::__eeObj, &__googleObj);

    auto cls = se::Class::create("EventBuilder", __googleObj, nullptr,
                                 _SE(jsb_EventBuilder_constructor));
    cls->defineFinalizeFunction(_SE(jsb_EventBuilder_finalize));

    cls->defineFunction("setCategory", _SE(jsb_EventBuilder_setCategory));
    cls->defineFunction("setAction", _SE(jsb_EventBuilder_setAction));
    cls->defineFunction("setLabel", _SE(jsb_EventBuilder_setLabel));
    cls->defineFunction("setValue", _SE(jsb_EventBuilder_setValue));
    cls->defineFunction("build", _SE(jsb_EventBuilder_build));

    cls->install();

    JSBClassType::registerClass<ee::GoogleEventBuilder>(cls);

    __jsb_EventBuilder_proto = cls->getProto();
    __jsb_EventBuilder_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace google

} // namespace ee
