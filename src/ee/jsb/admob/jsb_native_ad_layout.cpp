//
//  jsb_native_ad_layout.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#include "ee/jsb/admob/jsb_native_ad_layout.hpp"

#include "ee/AdMob.hpp"
#include "ee/jsb/admob/jsb_admob_bridge.hpp"
#include "ee/jsb/admob/jsb_native_ad_layout.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
template <>
AdMobNativeAdLayout get_value(const se::Value& value) {
    return *static_cast<AdMobNativeAdLayout*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, admob::NativeAdLayout& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace admob {
namespace {
se::Class* clazz = nullptr;

using Self = NativeAdLayout;

// clang-format off
constexpr auto finalize        = &core::makeFinalize<Self>;
constexpr auto constructor     = &core::makeConstructor<Self>;
constexpr auto setBody         = &core::makeInstanceMethod<&Self::setBody>;
constexpr auto setCallToAction = &core::makeInstanceMethod<&Self::setCallToAction>;
constexpr auto setHeadline     = &core::makeInstanceMethod<&Self::setHeadline>;
constexpr auto setIcon         = &core::makeInstanceMethod<&Self::setIcon>;
constexpr auto setImage        = &core::makeInstanceMethod<&Self::setImage>;
constexpr auto setMedia        = &core::makeInstanceMethod<&Self::setMedia>;
constexpr auto setPrice        = &core::makeInstanceMethod<&Self::setPrice>;
constexpr auto setStarRating   = &core::makeInstanceMethod<&Self::setStarRating>;
constexpr auto setStore        = &core::makeInstanceMethod<&Self::setStore>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(setBody)
SE_BIND_FUNC(setCallToAction)
SE_BIND_FUNC(setHeadline)
SE_BIND_FUNC(setIcon)
SE_BIND_FUNC(setImage)
SE_BIND_FUNC(setMedia)
SE_BIND_FUNC(setPrice)
SE_BIND_FUNC(setStarRating)
SE_BIND_FUNC(setStore)
} // namespace

bool register_native_ad_layout_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* admobObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("admob", eeObj, &admobObj);
    auto cls = se::Class::create("NativeAdLayout", admobObj, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setBody);
    EE_JSB_DEFINE_FUNCTION(cls, setCallToAction);
    EE_JSB_DEFINE_FUNCTION(cls, setHeadline);
    EE_JSB_DEFINE_FUNCTION(cls, setIcon);
    EE_JSB_DEFINE_FUNCTION(cls, setImage);
    EE_JSB_DEFINE_FUNCTION(cls, setMedia);
    EE_JSB_DEFINE_FUNCTION(cls, setPrice);
    EE_JSB_DEFINE_FUNCTION(cls, setStarRating);
    EE_JSB_DEFINE_FUNCTION(cls, setStore);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace admob
} // namespace ee
