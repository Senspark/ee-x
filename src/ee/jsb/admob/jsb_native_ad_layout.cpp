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
} // namespace core

namespace admob {
se::Class* __jsb_NativeAdLayout_class = nullptr;

const auto jsb_NativeAdLayout_finalize =
    &core::jsb_finalize<AdMobNativeAdLayout>;
const auto jsb_NativeAdLayout_constructor =
    &core::jsb_constructor<AdMobNativeAdLayout>;
const auto jsb_NativeAdLayout_setBody =
    &core::jsb_accessor_set<AdMobNativeAdLayout, &AdMobNativeAdLayout::setBody,
                            const std::string&>;
const auto jsb_NativeAdLayout_setCallToAction =
    &core::jsb_accessor_set<AdMobNativeAdLayout,
                            &AdMobNativeAdLayout::setCallToAction, std::string>;
const auto jsb_NativeAdLayout_setHeadline =
    &core::jsb_accessor_set<AdMobNativeAdLayout,
                            &AdMobNativeAdLayout::setHeadline, std::string>;
const auto jsb_NativeAdLayout_setIcon =
    &core::jsb_accessor_set<AdMobNativeAdLayout, &AdMobNativeAdLayout::setIcon,
                            std::string>;
const auto jsb_NativeAdLayout_setImage =
    &core::jsb_accessor_set<AdMobNativeAdLayout, &AdMobNativeAdLayout::setImage,
                            std::string>;
const auto jsb_NativeAdLayout_setMedia =
    &core::jsb_accessor_set<AdMobNativeAdLayout, &AdMobNativeAdLayout::setMedia,
                            std::string>;
const auto jsb_NativeAdLayout_setPrice =
    &core::jsb_accessor_set<AdMobNativeAdLayout, &AdMobNativeAdLayout::setPrice,
                            std::string>;
const auto jsb_NativeAdLayout_setStarRating =
    &core::jsb_accessor_set<AdMobNativeAdLayout,
                            &AdMobNativeAdLayout::setStarRating, std::string>;
const auto jsb_NativeAdLayout_setStore =
    &core::jsb_accessor_set<AdMobNativeAdLayout, &AdMobNativeAdLayout::setStore,
                            std::string>;

SE_BIND_FINALIZE_FUNC(jsb_NativeAdLayout_finalize)
SE_BIND_CTOR(jsb_NativeAdLayout_constructor, __jsb_NativeAdLayout_class,
             jsb_NativeAdLayout_finalize)
SE_BIND_FUNC(jsb_NativeAdLayout_setBody)
SE_BIND_FUNC(jsb_NativeAdLayout_setCallToAction)
SE_BIND_FUNC(jsb_NativeAdLayout_setHeadline)
SE_BIND_FUNC(jsb_NativeAdLayout_setIcon)
SE_BIND_FUNC(jsb_NativeAdLayout_setImage)
SE_BIND_FUNC(jsb_NativeAdLayout_setMedia)
SE_BIND_FUNC(jsb_NativeAdLayout_setPrice)
SE_BIND_FUNC(jsb_NativeAdLayout_setStarRating)
SE_BIND_FUNC(jsb_NativeAdLayout_setStore)

bool register_native_ad_layout_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* admobObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("admob", eeObj, &admobObj);
    auto cls = se::Class::create("NativeAdLayout", admobObj, nullptr,
                                 _SE(jsb_NativeAdLayout_constructor));
    cls->defineFinalizeFunction(_SE(jsb_NativeAdLayout_finalize));

    cls->defineFunction("setBody", _SE(jsb_NativeAdLayout_setBody));
    cls->defineFunction("setCallToAction",
                        _SE(jsb_NativeAdLayout_setCallToAction));
    cls->defineFunction("setHeadline", _SE(jsb_NativeAdLayout_setHeadline));
    cls->defineFunction("setIcon", _SE(jsb_NativeAdLayout_setIcon));
    cls->defineFunction("setImage", _SE(jsb_NativeAdLayout_setImage));
    cls->defineFunction("setMedia", _SE(jsb_NativeAdLayout_setMedia));
    cls->defineFunction("setPrice", _SE(jsb_NativeAdLayout_setPrice));
    cls->defineFunction("setStarRating", _SE(jsb_NativeAdLayout_setStarRating));
    cls->defineFunction("setStore", _SE(jsb_NativeAdLayout_setStore));

    cls->install();

    JSBClassType::registerClass<AdMobNativeAdLayout>(cls);

    __jsb_NativeAdLayout_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace admob
} // namespace ee
