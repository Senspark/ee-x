//
//  JsbFirebasePerformance.cpp
//  ee-x
//
//  Created by eps on 3/25/19.
//

#include "ee/jsb/firebase/JsbFirebasePerformance.hpp"

#include "ee/firebase/performance/FirebasePerformanceBridge.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace firebase {
namespace {
se::Class* clazz = nullptr;

using Self = Performance;

// clang-format off
constexpr auto constructor              = &core::makeConstructor<Self>;
constexpr auto finalize                 = &core::makeFinalize<Self>;
constexpr auto setDataCollectionEnabled = &core::makeMethod<&Self::setDataCollectionEnabled>;
constexpr auto isDataCollectionEnabled  = &core::makeMethod<&Self::isDataCollectionEnabled>;
constexpr auto startTrace               = &core::makeMethod<&Self::startTrace>;
constexpr auto newTrace                 = &core::makeMethod<&Self::newTrace>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_CTOR(constructor, clazz, finalize);
SE_BIND_FUNC(setDataCollectionEnabled);
SE_BIND_FUNC(isDataCollectionEnabled);
SE_BIND_FUNC(startTrace);
SE_BIND_FUNC(newTrace);
} // namespace

bool registerPerformance(se::Object* global) {
    auto firebase_ = core::getPath(global, "ee/firebase");
    auto cls =
        se::Class::create("Performance", firebase_, nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setDataCollectionEnabled);
    EE_JSB_DEFINE_FUNCTION(cls, isDataCollectionEnabled);
    EE_JSB_DEFINE_FUNCTION(cls, startTrace);
    EE_JSB_DEFINE_FUNCTION(cls, newTrace);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace firebase
} // namespace ee
