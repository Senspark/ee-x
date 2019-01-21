//
//  jsb_facebook_graph_request.cpp
//  ee-x
//
//  Created by senspark-dev on 12/12/18.
//

#include "ee/jsb/facebook/jsb_facebook_graph_request.hpp"

#include "ee/facebook/FacebookGraphRequest.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
template <>
facebook::GraphRequest get_value(const se::Value& value) {
    return *static_cast<facebook::GraphRequest*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, facebook::GraphRequest& input) {
    value.toObject()->setPrivateData(&input);
}
} // namespace core

namespace facebook {
namespace {
se::Class* clazz = nullptr;
} // namespace

constexpr auto jsb_GraphRequest_finalize = &core::jsb_finalize<GraphRequest>;
constexpr auto jsb_GraphRequest_constructor =
    &core::jsb_constructor<GraphRequest>;
constexpr auto jsb_GraphRequest_setPath =
    &core::jsb_accessor_set<GraphRequest, &GraphRequest::setPath,
                            const std::string&>;
constexpr auto jsb_GraphRequest_setParameter =
    &core::jsb_method_call<GraphRequest, &GraphRequest::setParameter,
                            const std::string&, const std::string&>;
const auto jsb_GraphRequest_toString =
    &core::jsb_method_get<GraphRequest, &GraphRequest::toString, std::string>;

SE_BIND_FINALIZE_FUNC(jsb_GraphRequest_finalize);
SE_BIND_CTOR(jsb_GraphRequest_constructor, clazz, jsb_GraphRequest_finalize);
SE_BIND_FUNC(jsb_GraphRequest_setPath);
SE_BIND_FUNC(jsb_GraphRequest_setParameter);
SE_BIND_FUNC(jsb_GraphRequest_toString);

bool register_facebook_graph_request_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("GraphRequest", facebookObj, nullptr,
                                 _SE(jsb_GraphRequest_constructor));
    cls->defineFinalizeFunction(_SE(jsb_GraphRequest_finalize));
    cls->defineFunction("setPath", _SE(jsb_GraphRequest_setPath));
    cls->defineFunction("setParameter", _SE(jsb_GraphRequest_setParameter));
    cls->defineFunction("toString", _SE(jsb_GraphRequest_toString));

    cls->install();

    JSBClassType::registerClass<GraphRequest>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
