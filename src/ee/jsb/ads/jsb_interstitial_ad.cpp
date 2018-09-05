//
//  jsb_interstitial_ad.cpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#include "jsb_interstitial_ad.hpp"

#include "jsb_core_common.hpp"

#include <ee/Ads.hpp>

#include <unordered_map>

namespace ee {
namespace ads {
static se::Object* __jsb_InterstitialAd_proto = nullptr;
static se::Class* __jsb_InterstitialAd_class = nullptr;

static std::unordered_map<std::shared_ptr<ee::IInterstitialAd>, se::Object*>
    __jsb_s_interstitialAds;
static std::vector<std::shared_ptr<ee::IInterstitialAd>>
    __jsb_s_interstitialAdArchive;
} // namespace ads

namespace core {
template <>
std::shared_ptr<ee::IInterstitialAd> get_value(const se::Value& value) {
    auto data = (ee::IInterstitialAd*)value.toObject()->getPrivateData();
    return std::shared_ptr<ee::IInterstitialAd>(data);
}

template <>
void set_value(se::Value& value, std::shared_ptr<ee::IInterstitialAd> input) {
    se::Object* obj = nullptr;
    if (ads::__jsb_s_interstitialAds.count(input) != 0) {
        obj = ads::__jsb_s_interstitialAds.at(input);
    } else {
        ads::__jsb_s_interstitialAdArchive.push_back(input);
        obj =
            se::Object::createObjectWithClass(ads::__jsb_InterstitialAd_class);
        obj->setPrivateData(input.get());
        //        obj->root();
    }
    value.setObject(obj);
}

template <>
bool jsb_finalize<IInterstitialAd>(se::State& s) {
    auto* interstitialAdPtr =
        static_cast<IInterstitialAd*>(s.nativeThisObject());
    auto iter =
        std::find_if(ads::__jsb_s_interstitialAdArchive.cbegin(),
                     ads::__jsb_s_interstitialAdArchive.cend(),
                     [=](const std::shared_ptr<IInterstitialAd>& ptr) -> bool {
                         return interstitialAdPtr == ptr.get();
                     });
    if (iter != ads::__jsb_s_interstitialAdArchive.cend()) {
        ads::__jsb_s_interstitialAdArchive.erase(iter);
    } else {
        delete interstitialAdPtr;
    }
    return true;
}
} // namespace core

namespace ads {
constexpr auto jsb_InterstitialAd_finalize =
    &ee::core::jsb_finalize<IInterstitialAd>;
constexpr auto jsb_InterstitialAd_isLoaded =
    &ee::core::jsb_accessor_get_on_ui_thread<IInterstitialAd,
                                             &IInterstitialAd::isLoaded, bool>;
constexpr auto jsb_InterstitialAd_load =
    &ee::core::jsb_method_call_on_ui_thread<IInterstitialAd,
                                            &IInterstitialAd::load>;
constexpr auto jsb_InterstitialAd_show =
    &ee::core::jsb_method_call_on_ui_thread<IInterstitialAd,
                                            &IInterstitialAd::show>;
constexpr auto jsb_InterstitialAd_setResultCallback =
    &ee::core::jsb_set_callback<IInterstitialAd,
                                &IInterstitialAd::setResultCallback>;

SE_BIND_FINALIZE_FUNC(jsb_InterstitialAd_finalize)
SE_BIND_FUNC(jsb_InterstitialAd_isLoaded)
SE_BIND_FUNC(jsb_InterstitialAd_load)
SE_BIND_FUNC(jsb_InterstitialAd_show)
SE_BIND_FUNC(jsb_InterstitialAd_setResultCallback)

bool register_interstitial_ad_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ads", globalObj, &adsObj);

    auto cls = se::Class::create("InterstitialAd", adsObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_InterstitialAd_finalize));

    cls->defineFunction("isLoaded", _SE(jsb_InterstitialAd_isLoaded));
    cls->defineFunction("load", _SE(jsb_InterstitialAd_load));
    cls->defineFunction("show", _SE(jsb_InterstitialAd_show));
    cls->defineFunction("setResultCallback",
                        _SE(jsb_InterstitialAd_setResultCallback));

    cls->install();

    JSBClassType::registerClass<ee::IInterstitialAd>(cls);

    __jsb_InterstitialAd_proto = cls->getProto();
    __jsb_InterstitialAd_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}

} // namespace ads
} // namespace ee
