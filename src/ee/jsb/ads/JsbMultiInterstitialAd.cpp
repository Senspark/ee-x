//
//  jsb_interstitial_ad.cpp
//  ee-x
//
//  Created by Kiet Le on 9/04/18.
//

#include "ee/jsb/ads/JsbMultiInterstitialAd.hpp"

#include <ee/ads/MultiInterstitialAd.hpp>
#include <ee/jsb/JsbCore.hpp>

#include "ee/jsb/ads/JsbIInterstitialAd.hpp"

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

bool registerJsbMultiInterstitialAd(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto parent = getIInterstitialClass()->getProto();
    auto cls = se::Class::create("MultiInterstitialAd", scope, parent,
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
