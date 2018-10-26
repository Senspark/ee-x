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
AdMobBannerAdSize get_value(const se::Value& value) {
    return static_cast<AdMobBannerAdSize>(value.toInt32());
}
} // namespace core

namespace admob {
se::Class* __jsb_AdMob_class = nullptr;

constexpr auto jsb_AdMob_finalize = &core::jsb_finalize<AdMob>;
constexpr auto jsb_AdMob_constructor = &core::jsb_constructor<AdMob>;
constexpr auto jsb_AdMob_initialize =
    &core::jsb_method_call_on_ui_thread_and_wait<AdMob, &AdMob::initialize,
                                                 const std::string&>;
constexpr auto jsb_AdMob_getEmulatorTestDeviceHash =
    &core::jsb_accessor_get<AdMob, &AdMob::getEmulatorTestDeviceHash,
                            const std::string&>;
constexpr auto jsb_AdMob_addTestDevice =
    &core::jsb_method_call_on_ui_thread<AdMob, &AdMob::addTestDevice,
                                        const std::string&>;
constexpr auto jsb_AdMob_createBannerAd =
    &core::jsb_method_get_on_ui_thread<AdMob, &AdMob::createBannerAd,
                                       std::shared_ptr<IAdView>,
                                       const std::string&, AdMobBannerAdSize>;
constexpr auto jsb_AdMob_createNativeAd = &core::jsb_method_get_on_ui_thread<
    AdMob, &AdMob::createNativeAd, std::shared_ptr<IAdView>, const std::string&,
    const std::string&, AdMobNativeAdLayout>;
constexpr auto jsb_AdMob_createInterstitialAd =
    &core::jsb_method_get_on_ui_thread<AdMob, &AdMob::createInterstitialAd,
                                       std::shared_ptr<IInterstitialAd>,
                                       const std::string&>;
constexpr auto jsb_AdMob_createRewardedVideo =
    &core::jsb_method_get_on_ui_thread<AdMob, &AdMob::createRewardedVideo,
                                       std::shared_ptr<IRewardedVideo>,
                                       const std::string&>;

SE_BIND_FINALIZE_FUNC(jsb_AdMob_finalize)
SE_BIND_CTOR(jsb_AdMob_constructor, __jsb_AdMob_class, jsb_AdMob_finalize)
SE_BIND_FUNC(jsb_AdMob_initialize)
SE_BIND_FUNC(jsb_AdMob_getEmulatorTestDeviceHash)
SE_BIND_FUNC(jsb_AdMob_addTestDevice)
SE_BIND_FUNC(jsb_AdMob_createBannerAd)
SE_BIND_FUNC(jsb_AdMob_createNativeAd)
SE_BIND_FUNC(jsb_AdMob_createInterstitialAd)
SE_BIND_FUNC(jsb_AdMob_createRewardedVideo)

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

    auto cls = se::Class::create("AdMob", admobObj, nullptr,
                                 _SE(jsb_AdMob_constructor));
    cls->defineFinalizeFunction(_SE(jsb_AdMob_finalize));

    cls->defineFunction("initialize", _SE(jsb_AdMob_initialize));
    cls->defineFunction("getEmulatorTestDeviceHash",
                        _SE(jsb_AdMob_getEmulatorTestDeviceHash));
    cls->defineFunction("addTestDevice", _SE(jsb_AdMob_addTestDevice));
    cls->defineFunction("createBannerAd", _SE(jsb_AdMob_createBannerAd));
    cls->defineFunction("createNativeAd", _SE(jsb_AdMob_createNativeAd));
    cls->defineFunction("createInterstitialAd",
                        _SE(jsb_AdMob_createInterstitialAd));
    cls->defineFunction("createRewardedVideo",
                        _SE(jsb_AdMob_createRewardedVideo));

    cls->install();

    JSBClassType::registerClass<AdMob>(cls);

    __jsb_AdMob_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace admob
} // namespace ee
