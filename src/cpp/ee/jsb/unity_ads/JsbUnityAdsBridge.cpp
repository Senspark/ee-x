//
// jsb_unity_ads_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/23/18
//
//

#include "ee/jsb/unity_ads/JsbUnityAdsBridge.hpp"

#include <ee/UnityAds.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace unity_ads {
namespace {
se::Class* clazz = nullptr;

using Self = Bridge;

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

bool registerJsbBridge(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = se::Class::create("UnityAds", scope, nullptr, _SE(constructor));
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
} // namespace unity_ads
} // namespace ee
