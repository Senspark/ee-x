//
//  jsb_metrics.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/26/18.
//

#include "ee/core/Metrics.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_core_common.hpp"
#include "jsb_metrics.hpp"

namespace ee {
namespace core {

static se::Object* __jsb_Metrics_proto = nullptr;
static se::Class* __jsb_Metrics_class = nullptr;
static std::unordered_map<const Metrics*, se::Object*> __jsb_s_Metrics;

template <>
void set_value(se::Value& value, ee::Metrics& input) {
    se::Object* obj = nullptr;
    if (__jsb_s_Metrics.count(&input) != 0) {
        obj = __jsb_s_Metrics[&input];
    } else {
        obj = se::Object::createObjectWithClass(__jsb_Metrics_class);
        obj->setPrivateData(new Metrics(input));
    }
    value.setObject(obj);
}

template <>
Metrics::ResolutionPolicy get_value(const se::Value& value) {
    return *static_cast<Metrics::ResolutionPolicy*>(
        value.toObject()->getPrivateData());
}

const auto jsb_Metrics_initialize =
    &jsb_static_call<(void (*)(float))(Metrics::initialize),float>;
const auto jsb_Metrics_getWinSize =
    &jsb_static_get<std::pair<float, float>, &Metrics::getWinSize>;
const auto jsb_Metrics_getFrameSize =
    &jsb_static_get<std::pair<float, float>, &Metrics::getFrameSize>;
const auto jsb_Metrics_getDensity =
    &jsb_static_get<float, &Metrics::getDensity>;
const auto jsb_Metrics_fromPoint =
    &jsb_static_get<Metrics&, &Metrics::fromPoint, float>;
const auto jsb_Metrics_fromPixel =
    &jsb_static_get<Metrics&, &Metrics::fromPixel, float>;
const auto jsb_Metrics_toPoint =
    &jsb_method_get<Metrics, &Metrics::toPoint, float>;
const auto jsb_Metrics_toPixel =
    &jsb_method_get<Metrics, &Metrics::toPixel, float>;
const auto jsb_Metrics_toDip = &jsb_method_get<Metrics, &Metrics::toDip, float>;

SE_BIND_FUNC(jsb_Metrics_initialize)
SE_BIND_FUNC(jsb_Metrics_getWinSize)
SE_BIND_FUNC(jsb_Metrics_getFrameSize)
SE_BIND_FUNC(jsb_Metrics_getDensity)
SE_BIND_FUNC(jsb_Metrics_fromPoint)
SE_BIND_FUNC(jsb_Metrics_fromPixel)
SE_BIND_FUNC(jsb_Metrics_toPoint)
SE_BIND_FUNC(jsb_Metrics_toPixel)
SE_BIND_FUNC(jsb_Metrics_toDip)

bool register_metrics_manual(se::Object* globalObj) {
    getOrCreatePlainObject_r("ee", globalObj, &__eeObj);
    getOrCreatePlainObject_r("core", __eeObj, &__coreObj);

    auto cls = se::Class::create("Metrics", __coreObj, nullptr, nullptr);
    cls->defineFunction("toPoint", _SE(jsb_Metrics_toPoint));
    cls->defineFunction("toPixel", _SE(jsb_Metrics_toPixel));
    cls->defineFunction("toDip", _SE(jsb_Metrics_toDip));

    // Install the class to JS virtual machine
    cls->install();

    JSBClassType::registerClass<ee::Metrics>(cls);

    __jsb_Metrics_proto = cls->getProto();
    __jsb_Metrics_class = cls;

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (__coreObj->getProperty("Metrics", &ctorVal) && ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction("initialize",
                                           _SE(jsb_Metrics_initialize));
        ctorVal.toObject()->defineFunction("getWinSize",
                                           _SE(jsb_Metrics_getWinSize));
        ctorVal.toObject()->defineFunction("getFrameSize",
                                           _SE(jsb_Metrics_getFrameSize));
        ctorVal.toObject()->defineFunction("getDensity",
                                           _SE(jsb_Metrics_getDensity));
        ctorVal.toObject()->defineFunction("fromPoint",
                                           _SE(jsb_Metrics_fromPoint));
        ctorVal.toObject()->defineFunction("fromPixel",
                                           _SE(jsb_Metrics_fromPixel));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace core
} // namespace ee