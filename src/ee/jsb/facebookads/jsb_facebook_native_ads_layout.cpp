//
//  jsb_facebook_native_ads_layout.cpp
//  ee-x
//
//  Created by Le Van Kiet on 8/21/18.
//

#include "ee/jsb/facebookads/jsb_facebook_native_ads_layout.hpp"

#include "ee/FacebookAds.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/facebookads/jsb_facebook_ads_bridge.hpp"

namespace ee {
namespace core {
template <>
facebook::NativeAdLayout get_value(const se::Value& value) {
    return *static_cast<facebook::NativeAdLayout*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, facebook::NativeAdLayout& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace facebook {
namespace {
se::Class* clazz = nullptr;

using Self = NativeAdLayout;

// clang-format off
constexpr auto constructor      = &core::makeConstructor<Self>;
constexpr auto finalize         = &core::makeFinalize<Self>;
constexpr auto setAdChoices     = &core::makeInstanceMethod<&Self::setAdChoices>;
constexpr auto setBody          = &core::makeInstanceMethod<&Self::setBody>;
constexpr auto setCallToAction  = &core::makeInstanceMethod<&Self::setCallToAction>;
constexpr auto setIcon          = &core::makeInstanceMethod<&Self::setIcon>;
constexpr auto setMedia         = &core::makeInstanceMethod<&Self::setMedia>;
constexpr auto setSocialContext = &core::makeInstanceMethod<&Self::setSocialContext>;
constexpr auto setTitle         = &core::makeInstanceMethod<&Self::setTitle>;
constexpr auto setCover         = &core::makeInstanceMethod<&Self::setCover>;
constexpr auto setSponsor       = &core::makeInstanceMethod<&Self::setSponsor>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(setAdChoices)
SE_BIND_FUNC(setBody)
SE_BIND_FUNC(setCallToAction)
SE_BIND_FUNC(setIcon)
SE_BIND_FUNC(setMedia)
SE_BIND_FUNC(setSocialContext)
SE_BIND_FUNC(setTitle)
SE_BIND_FUNC(setCover)
SE_BIND_FUNC(setSponsor)
} // namespace

bool register_facebook_native_ad_layout_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* facebookAdObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("facebookads", eeObj, &facebookAdObj);
    auto cls = se::Class::create("NativeAdLayout", facebookAdObj, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setAdChoices);
    EE_JSB_DEFINE_FUNCTION(cls, setBody);
    EE_JSB_DEFINE_FUNCTION(cls, setCallToAction);
    EE_JSB_DEFINE_FUNCTION(cls, setIcon);
    EE_JSB_DEFINE_FUNCTION(cls, setMedia);
    EE_JSB_DEFINE_FUNCTION(cls, setSocialContext);
    EE_JSB_DEFINE_FUNCTION(cls, setTitle);
    EE_JSB_DEFINE_FUNCTION(cls, setCover);
    EE_JSB_DEFINE_FUNCTION(cls, setSponsor);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
