//
//  jsb_google_analytics_event_builder.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/5/18.
//

#include "ee/jsb/google/jsb_google_analytics_event_builder.hpp"

#include <ee/GoogleAnalytics.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace core {
template <>
GoogleEventBuilder get_value(const se::Value& value) {
    return *static_cast<GoogleEventBuilder*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, google::EventBuilder& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace google {

se::Class* __jsb_EventBuilder_class = nullptr;

const auto jsb_EventBuilder_finalize = &core::jsb_finalize<GoogleEventBuilder>;
const auto jsb_EventBuilder_constructor =
    &core::jsb_constructor<GoogleEventBuilder, const std::string&,
                           const std::string&>;
const auto jsb_EventBuilder_setCategory = &core::jsb_accessor_set<
    GoogleEventBuilder, &GoogleEventBuilder::setCategory, const std::string&>;
const auto jsb_EventBuilder_setAction =
    &core::jsb_accessor_set<GoogleEventBuilder, &GoogleEventBuilder::setAction,
                            const std::string&>;
const auto jsb_EventBuilder_setLabel =
    &core::jsb_accessor_set<GoogleEventBuilder, &GoogleEventBuilder::setLabel,
                            const std::string&>;
const auto jsb_EventBuilder_setValue =
    &core::jsb_accessor_set<GoogleEventBuilder, &GoogleEventBuilder::setValue,
                            int>;
const auto jsb_EventBuilder_build =
    &core::jsb_method_get<GoogleEventBuilder, &GoogleEventBuilder::build,
                          const std::map<std::string, std::string>&>;

SE_BIND_FINALIZE_FUNC(jsb_EventBuilder_finalize)
SE_BIND_CTOR(jsb_EventBuilder_constructor, __jsb_EventBuilder_class,
             jsb_EventBuilder_finalize)
SE_BIND_FUNC(jsb_EventBuilder_setCategory)
SE_BIND_FUNC(jsb_EventBuilder_setAction)
SE_BIND_FUNC(jsb_EventBuilder_setLabel)
SE_BIND_FUNC(jsb_EventBuilder_setValue)
SE_BIND_FUNC(jsb_EventBuilder_build)

bool register_google_event_builder_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* googleObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("google", eeObj, &googleObj);

    auto cls = se::Class::create("EventBuilder", googleObj, nullptr,
                                 _SE(jsb_EventBuilder_constructor));
    cls->defineFinalizeFunction(_SE(jsb_EventBuilder_finalize));

    cls->defineFunction("setCategory", _SE(jsb_EventBuilder_setCategory));
    cls->defineFunction("setAction", _SE(jsb_EventBuilder_setAction));
    cls->defineFunction("setLabel", _SE(jsb_EventBuilder_setLabel));
    cls->defineFunction("setValue", _SE(jsb_EventBuilder_setValue));
    cls->defineFunction("build", _SE(jsb_EventBuilder_build));

    cls->install();

    JSBClassType::registerClass<GoogleEventBuilder>(cls);

    __jsb_EventBuilder_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace google
} // namespace ee
