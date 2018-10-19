//
//  jsb_facebook_native_ads_layout.cpp
//  ee-x
//
//  Created by Le Van Kiet on 8/21/18.
//

#include "ee/jsb/facebookads/jsb_facebook_native_ads_layout.hpp"

#include "ee/FacebookAds.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/facebookads/jsb_facebook_ads_bridge.hpp"

namespace ee {

namespace core {
template <>
ee::FacebookNativeAdLayout get_value(const se::Value& value) {
    return *static_cast<ee::FacebookNativeAdLayout*>(
        value.toObject()->getPrivateData());
}
} // namespace core

namespace facebook {

static se::Object* __jsb_NativeAdLayout_proto = nullptr;
static se::Class* __jsb_NativeAdLayout_class = nullptr;

const static auto jsb_NativeAdLayout_finalize =
    &ee::core::jsb_finalize<ee::FacebookNativeAdLayout>;
const static auto jsb_NativeAdLayout_constructor =
    &ee::core::jsb_constructor<ee::FacebookNativeAdLayout>;

const static auto jsb_NativeAdLayout_setAdChoices =
    &ee::core::jsb_accessor_set<ee::FacebookNativeAdLayout,
                                &ee::FacebookNativeAdLayout::setAdChoices,
                                const std::string&>;
const static auto jsb_NativeAdLayout_setBody =
    &ee::core::jsb_accessor_set<ee::FacebookNativeAdLayout,
                                &ee::FacebookNativeAdLayout::setBody,
                                const std::string&>;
const static auto jsb_NativeAdLayout_setCallToAction =
    &ee::core::jsb_accessor_set<ee::FacebookNativeAdLayout,
                                &ee::FacebookNativeAdLayout::setCallToAction,
                                const std::string&>;
const static auto jsb_NativeAdLayout_setIcon =
    &ee::core::jsb_accessor_set<ee::FacebookNativeAdLayout,
                                &ee::FacebookNativeAdLayout::setIcon,
                                const std::string&>;
const static auto jsb_NativeAdLayout_setMedia =
    &ee::core::jsb_accessor_set<ee::FacebookNativeAdLayout,
                                &ee::FacebookNativeAdLayout::setMedia,
                                const std::string&>;
const static auto jsb_NativeAdLayout_setSocialContext =
    &ee::core::jsb_accessor_set<ee::FacebookNativeAdLayout,
                                &ee::FacebookNativeAdLayout::setSocialContext,
                                const std::string&>;
const static auto jsb_NativeAdLayout_setTitle =
    &ee::core::jsb_accessor_set<ee::FacebookNativeAdLayout,
                                &ee::FacebookNativeAdLayout::setTitle,
                                const std::string&>;
const static auto jsb_NativeAdLayout_setCover =
    &ee::core::jsb_accessor_set<ee::FacebookNativeAdLayout,
                                &ee::FacebookNativeAdLayout::setCover,
                                const std::string&>;
const static auto jsb_NativeAdLayout_setSponsor =
    &ee::core::jsb_accessor_set<ee::FacebookNativeAdLayout,
                                &ee::FacebookNativeAdLayout::setSponsor,
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
    auto cls = se::Class::create("NativeAdLayout", __facebookAdObj, nullptr,
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

    JSBClassType::registerClass<ee::FacebookNativeAdLayout>(cls);

    __jsb_NativeAdLayout_proto = cls->getProto();
    __jsb_NativeAdLayout_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace facebook
} // namespace ee
