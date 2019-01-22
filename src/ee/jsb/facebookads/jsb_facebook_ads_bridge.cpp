//
//  jsb_facebook_bridge.cpp
//  ee-x
//
//  Created by Le Van Kiet on 8/21/18.
//

#include "ee/jsb/facebookads/jsb_facebook_ads_bridge.hpp"

#include "ee/FacebookAds.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
template <>
facebook::BannerAdSize get_value(const se::Value& value) {
    return static_cast<facebook::BannerAdSize>(value.toInt32());
}
} // namespace core

namespace facebook {
namespace {
se::Class* clazz = nullptr;

using Self = FacebookAds;

// clang-format off
constexpr auto constructor          = &core::makeConstructor<Self>;
constexpr auto finalize             = &core::makeFinalize<Self>;
constexpr auto getTestDeviceHash    = &core::makeInstanceMethod<&Self::getTestDeviceHash>;
constexpr auto addTestDevice        = &core::makeInstanceMethodOnUiThread<&Self::addTestDevice>;
constexpr auto createBannerAd       = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createBannerAd>;
constexpr auto createNativeAd       = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createNativeAd>;
constexpr auto createInterstitialAd = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createInterstitialAd>;
constexpr auto createRewardedVideo  = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createRewardedVideo>;
// clang-format on

SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(getTestDeviceHash)
SE_BIND_FUNC(addTestDevice)
SE_BIND_FUNC(createBannerAd)
SE_BIND_FUNC(createNativeAd)
SE_BIND_FUNC(createInterstitialAd)
SE_BIND_FUNC(createRewardedVideo)
} // namespace

bool register_facebookads_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* facebookAdObj = nullptr;
    se::Object* bannerAdSizeObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("facebookads", eeObj, &facebookAdObj);
    core::getOrCreatePlainObject_r("BannerAdSize", facebookAdObj,
                                   &bannerAdSizeObj);

    bannerAdSizeObj->setProperty("BannerHeight50",
                                 se::Value(static_cast<std::int32_t>(
                                     FacebookBannerAdSize::BannerHeight50)));
    bannerAdSizeObj->setProperty("BannerHeight90",
                                 se::Value(static_cast<std::int32_t>(
                                     FacebookBannerAdSize::BannerHeight90)));

    auto cls = se::Class::create("FacebookAds", facebookAdObj, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, getTestDeviceHash);
    EE_JSB_DEFINE_FUNCTION(cls, addTestDevice);
    EE_JSB_DEFINE_FUNCTION(cls, createBannerAd);
    EE_JSB_DEFINE_FUNCTION(cls, createNativeAd);
    EE_JSB_DEFINE_FUNCTION(cls, createInterstitialAd);
    EE_JSB_DEFINE_FUNCTION(cls, createRewardedVideo);

    cls->install();

    JSBClassType::registerClass<FacebookAds>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
