//
//  jsb_interstitial_ad.cpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#include "ee/jsb/ads/jsb_interstitial_ad.hpp"

#include <unordered_map>

#include <ee/Ads.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace ads {
se::Class* __jsb_InterstitialAd_class = nullptr;
std::unordered_map<std::shared_ptr<IInterstitialAd>, se::Object*>
    __jsb_s_interstitialAds;
std::vector<std::shared_ptr<IInterstitialAd>> __jsb_s_interstitialAdArchive;
} // namespace ads

namespace core {
template <>
std::shared_ptr<IInterstitialAd> get_value(const se::Value& value) {
    auto adPtr =
        static_cast<IInterstitialAd*>(value.toObject()->getPrivateData());
    auto iter =
        std::find_if(ads::__jsb_s_interstitialAdArchive.cbegin(),
                     ads::__jsb_s_interstitialAdArchive.cend(),
                     [=](const std::shared_ptr<IInterstitialAd>& ptr) -> bool {
                         return adPtr == ptr.get();
                     });
    if (iter != ads::__jsb_s_interstitialAdArchive.cend()) {
        return *iter;
    } else {
        return std::shared_ptr<IInterstitialAd>(adPtr);
    }
}

template <>
void set_value(se::Value& value, std::shared_ptr<IInterstitialAd> input) {
    if (input != nullptr) {
        se::Object* obj = nullptr;
        if (ads::__jsb_s_interstitialAds.count(input) != 0) {
            obj = ads::__jsb_s_interstitialAds.at(input);
        } else {
            ads::__jsb_s_interstitialAdArchive.push_back(input);
            obj = se::Object::createObjectWithClass(
                ads::__jsb_InterstitialAd_class);
            obj->setPrivateData(input.get());
            //        obj->root();
        }
        value.setObject(obj);
    } else {
        value.setNull();
    }
}

template <>
bool jsb_finalize<IInterstitialAd>(se::State& s) {
    auto interstitialAdPtr =
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
    &core::jsb_finalize<IInterstitialAd>;
constexpr auto jsb_InterstitialAd_isLoaded =
    &core::jsb_accessor_get_on_ui_thread<IInterstitialAd,
                                         &IInterstitialAd::isLoaded, bool>;
constexpr auto jsb_InterstitialAd_load =
    &core::jsb_method_call_on_ui_thread<IInterstitialAd,
                                        &IInterstitialAd::load>;
constexpr auto jsb_InterstitialAd_show =
    &core::jsb_method_call_on_ui_thread<IInterstitialAd,
                                        &IInterstitialAd::show>;
constexpr auto jsb_InterstitialAd_setResultCallback =
    &core::jsb_set_callback<IInterstitialAd,
                            &IInterstitialAd::setResultCallback>;

SE_BIND_FINALIZE_FUNC(jsb_InterstitialAd_finalize)
SE_BIND_FUNC(jsb_InterstitialAd_isLoaded)
SE_BIND_FUNC(jsb_InterstitialAd_load)
SE_BIND_FUNC(jsb_InterstitialAd_show)
SE_BIND_FUNC(jsb_InterstitialAd_setResultCallback)

se::Class* getIInterstitialClass() {
    CCASSERT(__jsb_InterstitialAd_class != nullptr,
             "__jsb_InterstitialAd_class is null");
    return __jsb_InterstitialAd_class;
}

bool register_interstitial_ad_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ads", eeObj, &adsObj);

    auto cls = se::Class::create("IInterstitialAd", adsObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_InterstitialAd_finalize));

    cls->defineFunction("isLoaded", _SE(jsb_InterstitialAd_isLoaded));
    cls->defineFunction("load", _SE(jsb_InterstitialAd_load));
    cls->defineFunction("show", _SE(jsb_InterstitialAd_show));
    cls->defineFunction("setResultCallback",
                        _SE(jsb_InterstitialAd_setResultCallback));

    cls->install();

    JSBClassType::registerClass<IInterstitialAd>(cls);

    __jsb_InterstitialAd_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace ads
} // namespace ee
