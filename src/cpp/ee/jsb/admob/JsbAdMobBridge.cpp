//
//  jsb_admob.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#include "ee/jsb/admob/JsbAdMobBridge.hpp"

#include <ee/AdMob.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace core {
template <>
admob::BannerAdSize get_value(const se::Value& value) {
    return static_cast<admob::BannerAdSize>(value.toInt32());
}
} // namespace core

namespace admob {
namespace {
se::Class* clazz = nullptr;

using Self = Bridge;

// clang-format off
constexpr auto constructor               = &core::makeConstructor<Self>;
constexpr auto finalize                  = &core::makeFinalize<Self>;
constexpr auto initialize                = &core::makeInstanceMethodOnUiThreadAndWait<&Self::initialize>;
constexpr auto getEmulatorTestDeviceHash = &core::makeInstanceMethod<&Self::getEmulatorTestDeviceHash>;
constexpr auto addTestDevice             = &core::makeInstanceMethodOnUiThread<&Self::addTestDevice>;
constexpr auto createBannerAd            = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createBannerAd>;
constexpr auto createNativeAd            = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createNativeAd>;
constexpr auto createInterstitialAd      = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createInterstitialAd>;
constexpr auto createRewardedVideo       = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createRewardedVideo>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(initialize)
SE_BIND_FUNC(getEmulatorTestDeviceHash)
SE_BIND_FUNC(addTestDevice)
SE_BIND_FUNC(createBannerAd)
SE_BIND_FUNC(createNativeAd)
SE_BIND_FUNC(createInterstitialAd)
SE_BIND_FUNC(createRewardedVideo)
} // namespace

bool registerJsbBridge(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    
    // Register AdMobBannerAdSize class.
    auto bannerAdSize = core::getPath(scope, "AdMobBannerAdSize");
    bannerAdSize->setProperty("Normal",
        se::Value(static_cast<std::int32_t>(AdMobBannerAdSize::Normal)));
    bannerAdSize->setProperty("Large",
        se::Value(static_cast<std::int32_t>(AdMobBannerAdSize::Large)));
    bannerAdSize->setProperty("Smart",
        se::Value(static_cast<std::int32_t>(AdMobBannerAdSize::Smart)));

    // Register AdMob class.
    auto cls = se::Class::create("AdMob", scope, nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, initialize);
    EE_JSB_DEFINE_FUNCTION(cls, getEmulatorTestDeviceHash);
    EE_JSB_DEFINE_FUNCTION(cls, addTestDevice);
    EE_JSB_DEFINE_FUNCTION(cls, createBannerAd);
    EE_JSB_DEFINE_FUNCTION(cls, createNativeAd);
    EE_JSB_DEFINE_FUNCTION(cls, createInterstitialAd);
    EE_JSB_DEFINE_FUNCTION(cls, createRewardedVideo);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace admob
} // namespace ee
