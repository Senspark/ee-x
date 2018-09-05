//
//  jsb_interstitial_ad.cpp
//  ee-x
//
//  Created by Kiet Le on 9/04/18.
//

#include "jsb_multi_interstitial_ad.hpp"

#include "jsb_core_common.hpp"

#include <ee/Ads.hpp>

#include <unordered_map>

namespace ee {
namespace ads {

static se::Object* __jsb_Multi_InterstitialAd_proto = nullptr;
static se::Class* __jsb_Multi_InterstitialAd_class = nullptr;

constexpr auto jsb_Multi_InterstitialAd_finalize =
    &ee::core::jsb_finalize<MultiInterstitialAd>;
constexpr static auto jsb_Multi_InterstitialAd_contructor =
    &ee::core::jsb_constructor<ee::MultiInterstitialAd>;
constexpr auto jsb_Multi_InterstitialAd_isLoaded =
    &ee::core::jsb_accessor_get_on_ui_thread<
        MultiInterstitialAd, &MultiInterstitialAd::isLoaded, bool>;
constexpr auto jsb_Multi_InterstitialAd_load =
    &ee::core::jsb_method_call_on_ui_thread<MultiInterstitialAd,
                                            &MultiInterstitialAd::load>;
constexpr auto jsb_Multi_InterstitialAd_show =
    &ee::core::jsb_method_call_on_ui_thread<MultiInterstitialAd,
                                            &MultiInterstitialAd::show>;
constexpr auto jsb_Multi_InterstitialAd_addItem =
    &ee::core::jsb_method_call_on_ui_thread<MultiInterstitialAd,
                                            &MultiInterstitialAd::addItem,
                                            std::shared_ptr<IInterstitialAd>>;
constexpr auto jsb_Multi_InterstitialAd_setResultCallback =
    &ee::core::jsb_set_callback<MultiInterstitialAd,
                                &MultiInterstitialAd::setResultCallback>;

SE_BIND_FINALIZE_FUNC(jsb_Multi_InterstitialAd_finalize)
SE_BIND_CTOR(jsb_Multi_InterstitialAd_contructor,
             __jsb_Multi_InterstitialAd_class,
             jsb_Multi_InterstitialAd_finalize)
SE_BIND_FUNC(jsb_Multi_InterstitialAd_isLoaded)
SE_BIND_FUNC(jsb_Multi_InterstitialAd_load)
SE_BIND_FUNC(jsb_Multi_InterstitialAd_show)
SE_BIND_FUNC(jsb_Multi_InterstitialAd_addItem)
SE_BIND_FUNC(jsb_Multi_InterstitialAd_setResultCallback)

bool register_multi_interstitial_ad_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ads", core::__eeObj, &adsObj);

    auto cls = se::Class::create("MultiInterstitialAd", adsObj, nullptr,
                                 _SE(jsb_Multi_InterstitialAd_contructor));
    cls->defineFinalizeFunction(_SE(jsb_Multi_InterstitialAd_finalize));

    cls->defineFunction("isLoaded", _SE(jsb_Multi_InterstitialAd_isLoaded));
    cls->defineFunction("load", _SE(jsb_Multi_InterstitialAd_load));
    cls->defineFunction("show", _SE(jsb_Multi_InterstitialAd_show));
    cls->defineFunction("addItem", _SE(jsb_Multi_InterstitialAd_addItem));
    cls->defineFunction("setResultCallback",
                        _SE(jsb_Multi_InterstitialAd_setResultCallback));

    cls->install();

    JSBClassType::registerClass<ee::MultiInterstitialAd>(cls);

    __jsb_Multi_InterstitialAd_proto = cls->getProto();
    __jsb_Multi_InterstitialAd_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}

} // namespace ads
} // namespace ee
