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
FacebookBannerAdSize get_value(const se::Value& value) {
    return static_cast<FacebookBannerAdSize>(value.toInt32());
}

} // namespace core

namespace facebook {

se::Object* __bannerAdSizeObj = nullptr;

se::Object* __jsb_FacebookAds_proto = nullptr;
se::Class* __jsb_FacebookAds_class = nullptr;

constexpr auto jsb_FacebookAds_finalize = &core::jsb_finalize<FacebookAds>;
constexpr auto jsb_FacebookAds_constructor =
    &core::jsb_constructor<FacebookAds>;
constexpr auto jsb_FacebookAds_getTestDeviceHash =
    &core::jsb_accessor_get<FacebookAds, &FacebookAds::getTestDeviceHash,
                            const std::string&>;
constexpr auto jsb_FacebookAds_addTestDevice =
    &core::jsb_method_call_on_ui_thread<
        FacebookAds, &FacebookAds::addTestDevice, const std::string&>;
constexpr auto jsb_FacebookAds_createBannerAd =
    &core::jsb_method_get_on_ui_thread<
        FacebookAds, &FacebookAds::createBannerAd, std::shared_ptr<IAdView>,
        const std::string&, FacebookBannerAdSize>;
constexpr auto jsb_FacebookAds_createNativeAd =
    &core::jsb_method_get_on_ui_thread<
        FacebookAds, &FacebookAds::createNativeAd, std::shared_ptr<IAdView>,
        const std::string&, const std::string&, FacebookNativeAdLayout>;
constexpr auto jsb_FacebookAds_createInterstitialAd =
    &core::jsb_method_get_on_ui_thread<
        FacebookAds, &FacebookAds::createInterstitialAd,
        std::shared_ptr<IInterstitialAd>, const std::string&>;
constexpr auto jsb_FacebookAds_createRewardedVideo =
    &core::jsb_method_get_on_ui_thread<
        FacebookAds, &FacebookAds::createRewardedVideo,
        std::shared_ptr<IRewardedVideo>, const std::string&>;

SE_BIND_FINALIZE_FUNC(jsb_FacebookAds_finalize)
SE_BIND_CTOR(jsb_FacebookAds_constructor, __jsb_FacebookAds_class,
             jsb_FacebookAds_finalize)
SE_BIND_FUNC(jsb_FacebookAds_getTestDeviceHash)
SE_BIND_FUNC(jsb_FacebookAds_addTestDevice)
SE_BIND_FUNC(jsb_FacebookAds_createBannerAd)
SE_BIND_FUNC(jsb_FacebookAds_createNativeAd)
SE_BIND_FUNC(jsb_FacebookAds_createInterstitialAd)
SE_BIND_FUNC(jsb_FacebookAds_createRewardedVideo)

bool register_facebookads_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* facebookAdObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("facebookads", eeObj, &facebookAdObj);
    core::getOrCreatePlainObject_r("BannerAdSize", facebookAdObj,
                                   &__bannerAdSizeObj);

    __bannerAdSizeObj->setProperty(
        "BannerHeight50",
        se::Value((std::int32_t)FacebookBannerAdSize::BannerHeight50));
    __bannerAdSizeObj->setProperty(
        "BannerHeight90",
        se::Value((std::int32_t)FacebookBannerAdSize::BannerHeight90));

    auto cls = se::Class::create("FacebookAds", facebookAdObj, nullptr,
                                 _SE(jsb_FacebookAds_constructor));
    cls->defineFinalizeFunction(_SE(jsb_FacebookAds_finalize));

    cls->defineFunction("getTestDeviceHash",
                        _SE(jsb_FacebookAds_getTestDeviceHash));
    cls->defineFunction("addTestDevice", _SE(jsb_FacebookAds_addTestDevice));
    cls->defineFunction("createBannerAd", _SE(jsb_FacebookAds_createBannerAd));
    cls->defineFunction("createNativeAd", _SE(jsb_FacebookAds_createNativeAd));
    cls->defineFunction("createInterstitialAd",
                        _SE(jsb_FacebookAds_createInterstitialAd));
    cls->defineFunction("createRewardedVideo",
                        _SE(jsb_FacebookAds_createRewardedVideo));

    cls->install();

    JSBClassType::registerClass<FacebookAds>(cls);

    __jsb_FacebookAds_proto = cls->getProto();
    __jsb_FacebookAds_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace facebook
} // namespace ee
