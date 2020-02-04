//
//  jsb_facebook_bridge.cpp
//  ee-x
//
//  Created by Le Van Kiet on 8/21/18.
//

#include "ee/jsb/facebook_ads/JsbFacebookAdsBridge.hpp"

#include <ee/FacebookAds.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace core {
template <>
facebook_ads::BannerAdSize get_value(const se::Value& value) {
    return static_cast<facebook_ads::BannerAdSize>(value.toInt32());
}
} // namespace core

namespace facebook_ads {
namespace {
se::Class* clazz = nullptr;

using Self = Bridge;

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

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(getTestDeviceHash)
SE_BIND_FUNC(addTestDevice)
SE_BIND_FUNC(createBannerAd)
SE_BIND_FUNC(createNativeAd)
SE_BIND_FUNC(createInterstitialAd)
SE_BIND_FUNC(createRewardedVideo)
} // namespace

bool registerJsbBridge(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    
    // Register FaceBookBannerAdSize class.
    auto bannerAdSize = core::getPath(scope, "FacebookBannerAdSize");
    bannerAdSize->setProperty("BannerHeight50",
        se::Value(static_cast<std::int32_t>(BannerAdSize::BannerHeight50)));
    bannerAdSize->setProperty("BannerHeight90",
        se::Value(static_cast<std::int32_t>(BannerAdSize::BannerHeight90)));

    // Register FacebookAds class.
    auto cls =
        se::Class::create("FacebookAds", scope, nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, getTestDeviceHash);
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
} // namespace facebook_ads
} // namespace ee
