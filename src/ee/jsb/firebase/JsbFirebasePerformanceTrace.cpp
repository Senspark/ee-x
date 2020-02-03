//
//  JsbFirebasePerformanceTrace.cpp
//  ee-x
//
//  Created by eps on 3/25/19.
//

#include "ee/jsb/firebase/JsbFirebasePerformanceTrace.hpp"

#include "ee/firebase/performance/FirebasePerformanceTrace.hpp"
#include "ee/jsb/core/SharedPtrHandler.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

using Self = ee::firebase::PerformanceTrace;

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<Self>> handler;
} // namespace

template <>
std::shared_ptr<Self> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value, std::shared_ptr<Self>& input) {
    handler->setValue(value, input);
}

template <>
bool makeFinalize<Self>(se::State& state) {
    return handler->finalize(state);
}
} // namespace core
namespace firebase {
namespace {
// clang-format off
constexpr auto finalize        = &core::makeFinalize<Self>;
constexpr auto start           = &core::makeMethod<&Self::start>;
constexpr auto stop            = &core::makeMethod<&Self::stop>;
constexpr auto putMetric       = &core::makeMethod<&Self::putMetric>;
constexpr auto incrementMetric = &core::makeMethod<&Self::incrementMetric>;
constexpr auto getLongMetric   = &core::makeMethod<&Self::getLongMetric>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(start);
SE_BIND_FUNC(stop);
SE_BIND_FUNC(putMetric);
SE_BIND_FUNC(incrementMetric);
SE_BIND_FUNC(getLongMetric);
} // namespace

bool registerPerformanceTrace(se::Object* global) {
    auto firebase_ = core::getPath(global, "ee/firebase");
    auto cls =
        se::Class::create("PerformanceTrace", firebase_, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, start);
    EE_JSB_DEFINE_FUNCTION(cls, stop);
    EE_JSB_DEFINE_FUNCTION(cls, putMetric);
    EE_JSB_DEFINE_FUNCTION(cls, incrementMetric);
    EE_JSB_DEFINE_FUNCTION(cls, getLongMetric);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    core::handler = core::SharedPtrHandler<Self>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace firebase
} // namespace ee
