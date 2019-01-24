//
//  jsb_interstitial_ad.cpp
//  ee-x
//
//  Created by Kiet Le on 9/04/18.
//

#include "ee/jsb/ads/jsb_multi_interstitial_ad.hpp"

#include <unordered_map>

#include "ee/ads/MultiInterstitialAd.hpp"
#include "ee/jsb/ads/jsb_interstitial_ad.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, MultiInterstitialAd& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace ads {
namespace {
se::Class* clazz = nullptr;

using Self = MultiInterstitialAd;

// clang-format off
constexpr auto finalize    = &core::makeFinalize<Self>;
constexpr auto constructor = &core::makeConstructor<Self>;
constexpr auto addItem     = &core::makeInstanceMethod<&Self::addItem>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(addItem)
} // namespace

bool register_multi_interstitial_ad_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    se::Object* eeObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ads", eeObj, &adsObj);

    auto cls = se::Class::create("MultiInterstitialAd", adsObj,
                                 getIInterstitialClass()->getProto(),
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, addItem);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ads
} // namespace ee
