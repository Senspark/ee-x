//
//  jsb_admob.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#include "jsb_admob_bridge.hpp"
#include "jsb_core_common.hpp"
#include "jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

#include <ee/Admob.hpp>

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, std::shared_ptr<ee::IAdView> input) {
// TODO: implementation
}

template <>
void set_value(se::Value& value, std::shared_ptr<ee::IInterstitialAd> input) {
// TODO: implementation
}

template <>
void set_value(se::Value& value, std::shared_ptr<ee::IRewardedVideo> input) {
// TODO: implementation
}
} // namespace core

namespace admob {

se::Object* __admobObj = nullptr;
static se::Object* __jsb_AdMob_proto = nullptr;
static se::Class* __jsb_AdMob_class = nullptr;

constexpr static auto jsb_AdMob_finalize = &ee::core::jsb_finalize<ee::AdMob>;
constexpr static auto jsb_AdMob_constructor =
    &ee::core::jsb_constructor<ee::AdMob>;
constexpr static auto jsb_AdMob_initialize =
    &ee::core::jsb_method_call<ee::AdMob, &ee::AdMob::initialize, std::string>;
constexpr static auto jsb_AdMob_getEmulatorTestDeviceHash =
    &ee::core::jsb_accessor_get<
        ee::AdMob, &ee::AdMob::getEmulatorTestDeviceHash, std::string>;
constexpr static auto jsb_AdMob_createBannerAd =
    &ee::core::jsb_method_get<ee::AdMob, &ee::AdMob::createBannerAd,
                              std::shared_ptr<IAdView>, std::string,
                              ee::AdMobBannerAdSize>;
constexpr static auto jsb_AdMob_createNativeAd =
    &ee::core::jsb_method_get<ee::AdMob, &ee::AdMob::createNativeAd,
                              std::shared_ptr<IAdView>, std::string,
                              std::string, ee::AdMobNativeAdLayout>;
constexpr static auto jsb_AdMob_createInterstitialAd =
    &ee::core::jsb_method_get<ee::AdMob, &ee::AdMob::createInterstitialAd,
                              std::shared_ptr<IInterstitialAd>, std::string>;
constexpr static auto jsb_AdMob_createRewardedVideo =
    &ee::core::jsb_method_get<ee::AdMob, &ee::AdMob::createRewardedVideo,
                              std::shared_ptr<IRewardedVideo>, std::string>;

SE_BIND_FINALIZE_FUNC(jsb_AdMob_finalize)
SE_BIND_CTOR(jsb_AdMob_constructor, __jsb_AdMob_class, jsb_AdMob_finalize)
SE_BIND_FUNC(jsb_AdMob_initialize)
SE_BIND_FUNC(jsb_AdMob_getEmulatorTestDeviceHash)
SE_BIND_FUNC(jsb_AdMob_createBannerAd)
SE_BIND_FUNC(jsb_AdMob_createNativeAd)
SE_BIND_FUNC(jsb_AdMob_createInterstitialAd)
SE_BIND_FUNC(jsb_AdMob_createRewardedVideo)

bool register_admob_manual(se::Object* globalObj) {
    core::getOrCreatePlainObject_r("admob", core::__eeObj, &__admobObj);

    auto cls = se::Class::create("AdMob", __admobObj, nullptr,
                                 _SE(jsb_AdMob_constructor));
    cls->defineFinalizeFunction(_SE(jsb_AdMob_finalize));
    
    cls->defineFunction("initialize", _SE(jsb_AdMob_initialize));
    cls->defineFunction("getEmulatorTestDeviceHash", _SE(jsb_AdMob_getEmulatorTestDeviceHash));
    cls->defineFunction("createBannerAd", _SE(jsb_AdMob_createBannerAd));
    cls->defineFunction("createNativeAd", _SE(jsb_AdMob_createNativeAd));
    cls->defineFunction("createInterstitialAd", _SE(jsb_AdMob_createInterstitialAd));
    cls->defineFunction("createRewardedVideo", _SE(jsb_AdMob_createRewardedVideo));

    cls->install();
    
    JSBClassType::registerClass<ee::AdMob>(cls);
    
    __jsb_AdMob_proto = cls->getProto();
    __jsb_AdMob_class = cls;
    
    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace admob
} // namespace ee
