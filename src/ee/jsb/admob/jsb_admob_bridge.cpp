//
//  jsb_admob.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_admob_bridge.hpp"
#include "jsb_core_common.hpp"
#include "jsb_logger.hpp"

#include "ee/AdMob.hpp"

namespace ee {
namespace core {
template <>
ee::AdMobBannerAdSize get_value(const se::Value& value) {
    return static_cast<ee::AdMobBannerAdSize>(value.toInt32());
}

} // namespace core

namespace admob {
se::Object* __admobObj = nullptr;
se::Object* __bannerAdSizeObj = nullptr;

static se::Object* __jsb_AdMob_proto = nullptr;
static se::Class* __jsb_AdMob_class = nullptr;

constexpr static auto jsb_AdMob_finalize = &ee::core::jsb_finalize<ee::AdMob>;
constexpr static auto jsb_AdMob_constructor =
    &ee::core::jsb_constructor<ee::AdMob>;
constexpr static auto jsb_AdMob_initialize =
    &ee::core::jsb_method_call_on_ui_thread_and_wait<
        ee::AdMob, &ee::AdMob::initialize, const std::string&>;
constexpr static auto jsb_AdMob_getEmulatorTestDeviceHash =
    &ee::core::jsb_accessor_get<
        ee::AdMob, &ee::AdMob::getEmulatorTestDeviceHash, std::string>;
constexpr static auto jsb_AdMob_addTestDevice =
    &ee::core::jsb_method_call_on_ui_thread<
        ee::AdMob, &ee::AdMob::addTestDevice, const std::string&>;
constexpr static auto jsb_AdMob_createBannerAd =
    &ee::core::jsb_method_get_on_ui_thread<
        ee::AdMob, &ee::AdMob::createBannerAd, std::shared_ptr<IAdView>,
        const std::string&, ee::AdMobBannerAdSize>;
constexpr static auto jsb_AdMob_createNativeAd =
    &ee::core::jsb_method_get_on_ui_thread<
        ee::AdMob, &ee::AdMob::createNativeAd, std::shared_ptr<IAdView>,
        const std::string&, const std::string&, ee::AdMobNativeAdLayout>;
constexpr static auto jsb_AdMob_createInterstitialAd =
    &ee::core::jsb_method_get_on_ui_thread<
        ee::AdMob, &ee::AdMob::createInterstitialAd,
        std::shared_ptr<IInterstitialAd>, const std::string&>;
constexpr static auto jsb_AdMob_createRewardedVideo =
    &ee::core::jsb_method_get_on_ui_thread<
        ee::AdMob, &ee::AdMob::createRewardedVideo,
        std::shared_ptr<IRewardedVideo>, const std::string&>;

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
    core::getOrCreatePlainObject_r("admob", core::__eeObj, &__admobObj);
    core::getOrCreatePlainObject_r("BannerAdSize", __admobObj,
                                   &__bannerAdSizeObj);

    __bannerAdSizeObj->setProperty(
        "Normal", se::Value((std::int32_t)ee::AdMobBannerAdSize::Normal));
    __bannerAdSizeObj->setProperty(
        "Large", se::Value((std::int32_t)ee::AdMobBannerAdSize::Large));
    __bannerAdSizeObj->setProperty(
        "Smart", se::Value((std::int32_t)ee::AdMobBannerAdSize::Smart));

    auto cls = se::Class::create("AdMob", __admobObj, nullptr,
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

    JSBClassType::registerClass<ee::AdMob>(cls);

    __jsb_AdMob_proto = cls->getProto();
    __jsb_AdMob_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace admob
} // namespace ee
