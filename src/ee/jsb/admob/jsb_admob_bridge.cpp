//
//  jsb_admob.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//
#include "ee/jsb/admob/jsb_admob_bridge.hpp"

#include "ee/AdMob.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

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

using Self = AdMob;

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

SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(initialize)
SE_BIND_FUNC(getEmulatorTestDeviceHash)
SE_BIND_FUNC(addTestDevice)
SE_BIND_FUNC(createBannerAd)
SE_BIND_FUNC(createNativeAd)
SE_BIND_FUNC(createInterstitialAd)
SE_BIND_FUNC(createRewardedVideo)
} // namespace

bool register_admob_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* admobObj = nullptr;
    se::Object* bannerAdSizeObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("admob", eeObj, &admobObj);
    core::getOrCreatePlainObject_r("BannerAdSize", admobObj, &bannerAdSizeObj);

    bannerAdSizeObj->setProperty("Normal", se::Value(static_cast<std::int32_t>(
                                               AdMobBannerAdSize::Normal)));
    bannerAdSizeObj->setProperty("Large", se::Value(static_cast<std::int32_t>(
                                              AdMobBannerAdSize::Large)));
    bannerAdSizeObj->setProperty("Smart", se::Value(static_cast<std::int32_t>(
                                              AdMobBannerAdSize::Smart)));

    auto cls = se::Class::create("AdMob", admobObj, nullptr, _SE(constructor));
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
