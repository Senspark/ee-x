//
//  jsb_cc_facebook_graph_request.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "jsb_cc_facebook_graph_request.hpp"

#include "ee/facebook/FacebookGraphRequest.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, FacebookGraphRequest& input) {
    set_value_from_pointer(value, &input);
}
    
} // namespace core

namespace facebook {
se::Class* __jsb_GraphRequest_class = nullptr;
const auto jsb_GraphRequest_finalize = &ee::core::jsb_finalize<GraphRequest>;
const auto jsb_GraphRequest_constructor =
    &ee::core::jsb_constructor<GraphRequest>;
constexpr auto jsb_GraphRequest_setPath =
    &ee::core::jsb_accessor_set<GraphRequest, &GraphRequest::setPath,
                                const std::string&>;
constexpr auto jsb_GraphRequest_setParameter =
    &ee::core::jsb_method_get<GraphRequest, &GraphRequest::setParameter,
                              GraphRequest&, const std::string&,
                              const std::string&>;
const auto jsb_GraphRequest_toString =
    &ee::core::jsb_method_get<GraphRequest, &GraphRequest::toString,
                              std::string>;

SE_BIND_FINALIZE_FUNC(jsb_GraphRequest_finalize);
SE_BIND_CTOR(jsb_GraphRequest_constructor, __jsb_GraphRequest_class,
             jsb_GraphRequest_finalize);
SE_BIND_FUNC(jsb_GraphRequest_setPath);
SE_BIND_FUNC(jsb_GraphRequest_setParameter);
SE_BIND_FUNC(jsb_GraphRequest_toString);

bool register_cc_facebook_graph_request_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    ee::core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    ee::core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("GraphRequest", facebookObj, nullptr,
                                 _SE(jsb_GraphRequest_constructor));
    cls->defineFinalizeFunction(_SE(jsb_GraphRequest_finalize));
    cls->defineFunction("setPath", _SE(jsb_GraphRequest_setPath));
    cls->defineFunction("setParameter", _SE(jsb_GraphRequest_setParameter));
    cls->defineFunction("toString", _SE(jsb_GraphRequest_toString));

    cls->install();

    JSBClassType::registerClass<GraphRequest>(cls);

    __jsb_GraphRequest_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
