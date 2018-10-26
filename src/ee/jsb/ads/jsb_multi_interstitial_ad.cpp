//
//  jsb_interstitial_ad.cpp
//  ee-x
//
//  Created by Kiet Le on 9/04/18.
//

#include "ee/jsb/ads/jsb_multi_interstitial_ad.hpp"

#include <unordered_map>

#include <ee/Ads.hpp>

#include "ee/jsb/ads/jsb_interstitial_ad.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace ads {

se::Class* __jsb_Multi_InterstitialAd_class = nullptr;

constexpr auto jsb_Multi_InterstitialAd_finalize =
    &core::jsb_finalize<MultiInterstitialAd>;
constexpr auto jsb_Multi_InterstitialAd_contructor =
    &core::jsb_constructor<MultiInterstitialAd>;
constexpr auto jsb_Multi_InterstitialAd_addItem =
    &core::jsb_method_call_on_ui_thread<MultiInterstitialAd,
                                        &MultiInterstitialAd::addItem,
                                        std::shared_ptr<IInterstitialAd>>;

SE_BIND_FINALIZE_FUNC(jsb_Multi_InterstitialAd_finalize)
SE_BIND_CTOR(jsb_Multi_InterstitialAd_contructor,
             __jsb_Multi_InterstitialAd_class,
             jsb_Multi_InterstitialAd_finalize)
SE_BIND_FUNC(jsb_Multi_InterstitialAd_addItem)

bool register_multi_interstitial_ad_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    se::Object* eeObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ads", eeObj, &adsObj);

    auto cls = se::Class::create("MultiInterstitialAd", adsObj,
                                 getIInterstitialClass()->getProto(),
                                 _SE(jsb_Multi_InterstitialAd_contructor));
    cls->defineFinalizeFunction(_SE(jsb_Multi_InterstitialAd_finalize));

    cls->defineFunction("addItem", _SE(jsb_Multi_InterstitialAd_addItem));

    cls->install();

    JSBClassType::registerClass<MultiInterstitialAd>(cls);

    __jsb_Multi_InterstitialAd_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}

} // namespace ads
} // namespace ee
