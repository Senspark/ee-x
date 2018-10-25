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
FacebookNativeAdLayout get_value(const se::Value& value) {
    return *static_cast<FacebookNativeAdLayout*>(
        value.toObject()->getPrivateData());
}
} // namespace core

namespace facebook {

se::Object* __jsb_NativeAdLayout_proto = nullptr;
se::Class* __jsb_NativeAdLayout_class = nullptr;

const auto jsb_NativeAdLayout_finalize =
    &core::jsb_finalize<FacebookNativeAdLayout>;
const auto jsb_NativeAdLayout_constructor =
    &core::jsb_constructor<FacebookNativeAdLayout>;

const auto jsb_NativeAdLayout_setAdChoices =
    &core::jsb_accessor_set<FacebookNativeAdLayout,
                            &FacebookNativeAdLayout::setAdChoices,
                            const std::string&>;
const auto jsb_NativeAdLayout_setBody =
    &core::jsb_accessor_set<FacebookNativeAdLayout,
                            &FacebookNativeAdLayout::setBody,
                            const std::string&>;
const auto jsb_NativeAdLayout_setCallToAction =
    &core::jsb_accessor_set<FacebookNativeAdLayout,
                            &FacebookNativeAdLayout::setCallToAction,
                            const std::string&>;
const auto jsb_NativeAdLayout_setIcon =
    &core::jsb_accessor_set<FacebookNativeAdLayout,
                            &FacebookNativeAdLayout::setIcon,
                            const std::string&>;
const auto jsb_NativeAdLayout_setMedia =
    &core::jsb_accessor_set<FacebookNativeAdLayout,
                            &FacebookNativeAdLayout::setMedia,
                            const std::string&>;
const auto jsb_NativeAdLayout_setSocialContext =
    &core::jsb_accessor_set<FacebookNativeAdLayout,
                            &FacebookNativeAdLayout::setSocialContext,
                            const std::string&>;
const auto jsb_NativeAdLayout_setTitle =
    &core::jsb_accessor_set<FacebookNativeAdLayout,
                            &FacebookNativeAdLayout::setTitle,
                            const std::string&>;
const auto jsb_NativeAdLayout_setCover =
    &core::jsb_accessor_set<FacebookNativeAdLayout,
                            &FacebookNativeAdLayout::setCover,
                            const std::string&>;
const auto jsb_NativeAdLayout_setSponsor =
    &core::jsb_accessor_set<FacebookNativeAdLayout,
                            &FacebookNativeAdLayout::setSponsor,
                            const std::string&>;

SE_BIND_FINALIZE_FUNC(jsb_NativeAdLayout_finalize)
SE_BIND_CTOR(jsb_NativeAdLayout_constructor, __jsb_NativeAdLayout_class,
             jsb_NativeAdLayout_finalize)

SE_BIND_FUNC(jsb_NativeAdLayout_setAdChoices)
SE_BIND_FUNC(jsb_NativeAdLayout_setBody)
SE_BIND_FUNC(jsb_NativeAdLayout_setCallToAction)
SE_BIND_FUNC(jsb_NativeAdLayout_setIcon)
SE_BIND_FUNC(jsb_NativeAdLayout_setMedia)
SE_BIND_FUNC(jsb_NativeAdLayout_setSocialContext)
SE_BIND_FUNC(jsb_NativeAdLayout_setTitle)
SE_BIND_FUNC(jsb_NativeAdLayout_setCover)
SE_BIND_FUNC(jsb_NativeAdLayout_setSponsor)

bool register_facebook_native_ad_layout_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* facebookAdObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("facebookads", eeObj, &facebookAdObj);
    auto cls = se::Class::create("NativeAdLayout", facebookAdObj, nullptr,
                                 _SE(jsb_NativeAdLayout_constructor));
    cls->defineFinalizeFunction(_SE(jsb_NativeAdLayout_finalize));

    cls->defineFunction("setAdChoices", _SE(jsb_NativeAdLayout_setAdChoices));
    cls->defineFunction("setBody", _SE(jsb_NativeAdLayout_setBody));
    cls->defineFunction("setCallToAction",
                        _SE(jsb_NativeAdLayout_setCallToAction));
    cls->defineFunction("setIcon", _SE(jsb_NativeAdLayout_setIcon));
    cls->defineFunction("setMedia", _SE(jsb_NativeAdLayout_setMedia));
    cls->defineFunction("setSocialContext",
                        _SE(jsb_NativeAdLayout_setSocialContext));
    cls->defineFunction("setTitle", _SE(jsb_NativeAdLayout_setTitle));
    cls->defineFunction("setCover", _SE(jsb_NativeAdLayout_setCover));
    cls->defineFunction("setSponsor", _SE(jsb_NativeAdLayout_setSponsor));

    cls->install();

    JSBClassType::registerClass<FacebookNativeAdLayout>(cls);

    __jsb_NativeAdLayout_proto = cls->getProto();
    __jsb_NativeAdLayout_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace facebook
} // namespace ee
