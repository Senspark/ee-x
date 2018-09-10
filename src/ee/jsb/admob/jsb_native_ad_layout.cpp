//
//  jsb_native_ad_layout.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#include "jsb_native_ad_layout.hpp"

#include "jsb_admob_bridge.hpp"
#include "jsb_core_common.hpp"

#include "ee/AdMob.hpp"

namespace ee {

namespace core {
template <> ee::AdMobNativeAdLayout get_value(const se::Value& value) {
    return *static_cast<ee::AdMobNativeAdLayout*>(
        value.toObject()->getPrivateData());
}
} // namespace core

namespace admob {

static se::Object* __jsb_NativeAdLayout_proto = nullptr;
static se::Class* __jsb_NativeAdLayout_class = nullptr;

const static auto jsb_NativeAdLayout_finalize =
    &ee::core::jsb_finalize<ee::AdMobNativeAdLayout>;
const static auto jsb_NativeAdLayout_constructor =
    &ee::core::jsb_constructor<ee::AdMobNativeAdLayout>;

const static auto jsb_NativeAdLayout_setBody =
    &ee::core::jsb_accessor_set<ee::AdMobNativeAdLayout,
                                &ee::AdMobNativeAdLayout::setBody,
                                const std::string&>;
const static auto jsb_NativeAdLayout_setCallToAction =
    &ee::core::jsb_accessor_set<ee::AdMobNativeAdLayout,
                                &ee::AdMobNativeAdLayout::setCallToAction,
                                std::string>;
const static auto jsb_NativeAdLayout_setHeadline =
    &ee::core::jsb_accessor_set<ee::AdMobNativeAdLayout,
                                &ee::AdMobNativeAdLayout::setHeadline,
                                std::string>;
const static auto jsb_NativeAdLayout_setIcon =
    &ee::core::jsb_accessor_set<ee::AdMobNativeAdLayout,
                                &ee::AdMobNativeAdLayout::setIcon,
                                std::string>;
const static auto jsb_NativeAdLayout_setImage = &ee::core::jsb_accessor_set<
    ee::AdMobNativeAdLayout, &ee::AdMobNativeAdLayout::setImage, std::string>;
const static auto jsb_NativeAdLayout_setMedia = &ee::core::jsb_accessor_set<
    ee::AdMobNativeAdLayout, &ee::AdMobNativeAdLayout::setMedia, std::string>;
const static auto jsb_NativeAdLayout_setPrice = &ee::core::jsb_accessor_set<
    ee::AdMobNativeAdLayout, &ee::AdMobNativeAdLayout::setPrice, std::string>;
const static auto jsb_NativeAdLayout_setStarRating =
    &ee::core::jsb_accessor_set<ee::AdMobNativeAdLayout,
                                &ee::AdMobNativeAdLayout::setStarRating,
                                std::string>;
const static auto jsb_NativeAdLayout_setStore = &ee::core::jsb_accessor_set<
    ee::AdMobNativeAdLayout, &ee::AdMobNativeAdLayout::setStore, std::string>;

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
    auto cls = se::Class::create("NativeAdLayout", __admobObj, nullptr,
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

    JSBClassType::registerClass<ee::AdMobNativeAdLayout>(cls);

    __jsb_NativeAdLayout_proto = cls->getProto();
    __jsb_NativeAdLayout_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace admob
} // namespace ee
