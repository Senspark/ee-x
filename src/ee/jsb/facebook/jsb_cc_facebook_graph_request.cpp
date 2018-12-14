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
namespace facebook {
se::Class* __jsb_GraphRequest_class = nullptr;
std::unordered_map<std::shared_ptr<GraphRequest>, se::Object*> __jsb_s_graphRequests;
std::vector<std::shared_ptr<GraphRequest>> __jsb_s_graphRequestsArchive;
} // namespace facebook

namespace core {
template <>
std::shared_ptr<facebook::GraphRequest> get_value(const se::Value& value) {
    auto itemPtr = static_cast<facebook::GraphRequest*>(
        value.toObject()->getPrivateData());
    auto iter = std::find_if(
        facebook::__jsb_s_graphRequestsArchive.cbegin(),
        facebook::__jsb_s_graphRequestsArchive.cend(),
        [=](const std::shared_ptr<facebook::GraphRequest>& ptr) -> bool {
            return itemPtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_graphRequestsArchive.cend()) {
        return *iter;
    } else {
        return std::shared_ptr<facebook::GraphRequest>(itemPtr);
    }
}

template <>
void set_value(se::Value& value,
               std::shared_ptr<facebook::GraphRequest> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (facebook::__jsb_s_graphRequests.count(input) != 0) {
            obj = facebook::__jsb_s_graphRequests.at(input);
        } else {
            facebook::__jsb_s_graphRequestsArchive.push_back(input);
            obj = se::Object::createObjectWithClass(
                facebook::__jsb_GraphRequest_class);
            obj->setPrivateData(input.get());
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
bool jsb_finalize<facebook::GraphRequest>(se::State& s) {
    auto itemPtr = static_cast<facebook::GraphRequest*>(s.nativeThisObject());
    auto iter = std::find_if(
        facebook::__jsb_s_graphRequestsArchive.cbegin(),
        facebook::__jsb_s_graphRequestsArchive.cend(),
        [=](const std::shared_ptr<facebook::GraphRequest>& ptr) -> bool {
            return itemPtr == ptr.get();
        });
    if (iter != facebook::__jsb_s_graphRequestsArchive.cend()) {
        facebook::__jsb_s_graphRequestsArchive.erase(iter);
    } else {
        delete itemPtr;
    }
    return true;
}
} // namespace core

namespace facebook {
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

bool register_facebook_graph_request_manual(se::Object* globalObject) {
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
