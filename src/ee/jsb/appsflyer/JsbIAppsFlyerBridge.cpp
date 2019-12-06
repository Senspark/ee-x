//
//  JsbIAppsFlyerBridge.cpp
//  ee-x
//
//  Created by eps on 12/05/19.
//

#include "ee/jsb/appsflyer/JsbIAppsFlyerBridge.hpp"

#include "ee/appsflyer/IAppsFlyerBridge.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace appsflyer {
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
    auto appsflyer_ = core::getPath(global, "ee/appsflyer");

    auto cls = se::Class::create("IBridge", appsflyer_, nullptr, nullptr);
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
} // namespace appsflyer
} // namespace ee
