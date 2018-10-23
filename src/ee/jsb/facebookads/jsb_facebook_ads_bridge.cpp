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
ee::FacebookBannerAdSize get_value(const se::Value& value) {
    return static_cast<ee::FacebookBannerAdSize>(value.toInt32());
}

} // namespace core

namespace facebook {

se::Object* __facebookAdObj = nullptr;
se::Object* __bannerAdSizeObj = nullptr;

static se::Object* __jsb_FacebookAds_proto = nullptr;
static se::Class* __jsb_FacebookAds_class = nullptr;

constexpr static auto jsb_FacebookAds_finalize =
    &ee::core::jsb_finalize<ee::FacebookAds>;
constexpr static auto jsb_FacebookAds_constructor =
    &ee::core::jsb_constructor<ee::FacebookAds>;
constexpr static auto jsb_FacebookAds_getTestDeviceHash =
    &ee::core::jsb_accessor_get<ee::FacebookAds,
                                &ee::FacebookAds::getTestDeviceHash,
                                const std::string&>;
constexpr static auto jsb_FacebookAds_addTestDevice =
    &ee::core::jsb_method_call_on_ui_thread<
        ee::FacebookAds, &ee::FacebookAds::addTestDevice, const std::string&>;
constexpr static auto jsb_FacebookAds_createBannerAd =
    &ee::core::jsb_method_get_on_ui_thread<
        ee::FacebookAds, &ee::FacebookAds::createBannerAd,
        std::shared_ptr<IAdView>, const std::string&, ee::FacebookBannerAdSize>;
constexpr static auto jsb_FacebookAds_createNativeAd =
    &ee::core::jsb_method_get_on_ui_thread<
        ee::FacebookAds, &ee::FacebookAds::createNativeAd,
        std::shared_ptr<IAdView>, const std::string&, const std::string&,
        ee::FacebookNativeAdLayout>;
constexpr static auto jsb_FacebookAds_createInterstitialAd =
    &ee::core::jsb_method_get_on_ui_thread<
        ee::FacebookAds, &ee::FacebookAds::createInterstitialAd,
        std::shared_ptr<IInterstitialAd>, const std::string&>;
constexpr static auto jsb_FacebookAds_createRewardedVideo =
    &ee::core::jsb_method_get_on_ui_thread<
        ee::FacebookAds, &ee::FacebookAds::createRewardedVideo,
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
    core::getOrCreatePlainObject_r("facebookads", core::__eeObj,
                                   &__facebookAdObj);
    core::getOrCreatePlainObject_r("BannerAdSize", __facebookAdObj,
                                   &__bannerAdSizeObj);

    __bannerAdSizeObj->setProperty(
        "BannerHeight50",
        se::Value((std::int32_t)ee::FacebookBannerAdSize::BannerHeight50));
    __bannerAdSizeObj->setProperty(
        "BannerHeight90",
        se::Value((std::int32_t)ee::FacebookBannerAdSize::BannerHeight90));

    auto cls = se::Class::create("FacebookAds", __facebookAdObj, nullptr,
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

    JSBClassType::registerClass<ee::FacebookAds>(cls);

    __jsb_FacebookAds_proto = cls->getProto();
    __jsb_FacebookAds_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace facebook
} // namespace ee
