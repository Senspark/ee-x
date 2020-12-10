//
//  JsbIAppsFlyerBridge.cpp
//  ee-x
//
//  Created by eps on 12/05/19.
//

#include "ee/jsb/apps_flyer/JsbIAppsFlyerBridge.hpp"

#include <ee/apps_flyer/IAppsFlyerBridge.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace apps_flyer {
namespace {
se::Class* clazz = nullptr;

using Self = IBridge;

// clang-format off
constexpr auto finalize              = &core::makeFinalize<Self>;
constexpr auto initialize            = &core::makeInstanceMethodOnUiThread<&Self::initialize>;
constexpr auto startTracking         = &core::makeInstanceMethodOnUiThread<&Self::startTracking>;
constexpr auto getDeviceId           = &core::makeMethod<&Self::getDeviceId>;
constexpr auto setDebugEnabled       = &core::makeInstanceMethodOnUiThread<&Self::setDebugEnabled>;
constexpr auto trackEvent            = &core::makeMethod<&Self::trackEvent>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(initialize);
SE_BIND_FUNC(startTracking);
SE_BIND_FUNC(getDeviceId);
SE_BIND_FUNC(setDebugEnabled);
SE_BIND_FUNC(trackEvent);
} // namespace

se::Class* getIBridgeClass() {
    return clazz;
}

bool registerJsbIBridge(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = se::Class::create("IAppsFlyer", scope, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, initialize);
    EE_JSB_DEFINE_FUNCTION(cls, startTracking);
    EE_JSB_DEFINE_FUNCTION(cls, getDeviceId);
    EE_JSB_DEFINE_FUNCTION(cls, setDebugEnabled);
    EE_JSB_DEFINE_FUNCTION(cls, trackEvent);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace apps_flyer
} // namespace ee
