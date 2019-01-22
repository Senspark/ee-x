//
// jsb_unity_ads_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//
//

#include "ee/jsb/unityads/jsb_unity_ads_bridge.hpp"

#include "ee/UnityAds.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace unityads {
namespace {
se::Class* clazz = nullptr;

using Self = UnityAds;

// clang-format off
constexpr auto finalize             = &core::makeFinalize<Self>;
constexpr auto constructor          = &core::makeConstructor<Self>;
constexpr auto initialize           = &core::makeInstanceMethodOnUiThreadAndWait<&Self::initialize>;
constexpr auto setDebugModeEnabled  = &core::makeInstanceMethodOnUiThread<&Self::setDebugModeEnabled>;
constexpr auto createRewardedVideo  = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createRewardedVideo>;
constexpr auto createInterstitialAd = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createInterstitialAd>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(initialize)
SE_BIND_FUNC(setDebugModeEnabled)
SE_BIND_FUNC(createRewardedVideo)
SE_BIND_FUNC(createInterstitialAd)
} // namespace

bool register_unity_ads_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* unityadsObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("unityads", eeObj, &unityadsObj);

    auto cls =
        se::Class::create("UnityAds", unityadsObj, nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));
    EE_JSB_DEFINE_FUNCTION(cls, initialize);
    EE_JSB_DEFINE_FUNCTION(cls, setDebugModeEnabled);
    EE_JSB_DEFINE_FUNCTION(cls, createRewardedVideo);
    EE_JSB_DEFINE_FUNCTION(cls, createInterstitialAd);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace unityads
} // namespace ee
