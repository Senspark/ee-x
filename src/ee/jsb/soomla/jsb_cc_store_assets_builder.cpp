//
//  jsb_cc_store_assets_builder.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/13/18.
//

#include "ee/jsb/soomla/jsb_cc_store_assets_builder.hpp"

#include "Soomla/CCStoreAssetsBuilder.h"
#include "Soomla/Cocos2dxCore.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
template <>
soomla::CCVirtualCurrency* get_value(const se::Value& value) {
    return static_cast<soomla::CCVirtualCurrency*>(
        value.toObject()->getPrivateData());
}

template <>
soomla::CCVirtualGood* get_value(const se::Value& value) {
    return static_cast<soomla::CCVirtualGood*>(
        value.toObject()->getPrivateData());
}

template <>
soomla::CCVirtualCurrencyPack* get_value(const se::Value& value) {
    return static_cast<soomla::CCVirtualCurrencyPack*>(
        value.toObject()->getPrivateData());
}

template <>
soomla::CCVirtualCategory* get_value(const se::Value& value) {
    return static_cast<soomla::CCVirtualCategory*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, soomla::CCStoreAssetsBuilder& input) {
    set_value_from_pointer(value, &input);
}

template <>
void set_value(se::Value& value, soomla::CCStoreAssets* input) {
    set_value_from_pointer(value, input);
}
} // namespace core
} // namespace ee

namespace soomla {
se::Class* __jsb_CCStoreAssetsBuilder_class = nullptr;

const auto jsb_CCStoreAssetsBuilder_finalize =
    &ee::core::jsb_finalize<CCStoreAssetsBuilder>;
const auto jsb_CCStoreAssetsBuilder_constructor =
    &ee::core::jsb_constructor<CCStoreAssetsBuilder>;
const auto jsb_CCStoreAssetsBuilder_setVersion = &ee::core::jsb_accessor_set<
    CCStoreAssetsBuilder, &CCStoreAssetsBuilder::setVersion, std::int32_t>;
const auto jsb_CCStoreAssetsBuilder_addCurrency =
    &ee::core::jsb_accessor_set<CCStoreAssetsBuilder,
                                &CCStoreAssetsBuilder::addCurrency,
                                CCVirtualCurrency*>;
const auto jsb_CCStoreAssetsBuilder_addGood =
    &ee::core::jsb_accessor_set<CCStoreAssetsBuilder,
                                &CCStoreAssetsBuilder::addGood, CCVirtualGood*>;
const auto jsb_CCStoreAssetsBuilder_addCurrencyPack =
    &ee::core::jsb_accessor_set<CCStoreAssetsBuilder,
                                &CCStoreAssetsBuilder::addCurrencyPack,
                                CCVirtualCurrencyPack*>;
const auto jsb_CCStoreAssetsBuilder_addCategorie =
    &ee::core::jsb_accessor_set<CCStoreAssetsBuilder,
                                &CCStoreAssetsBuilder::addCategorie,
                                CCVirtualCategory*>;
const auto jsb_CCStoreAssetsBuilder_build =
    &ee::core::jsb_method_get<CCStoreAssetsBuilder,
                              &CCStoreAssetsBuilder::build, CCStoreAssets*>;

SE_BIND_FINALIZE_FUNC(jsb_CCStoreAssetsBuilder_finalize)
SE_BIND_CTOR(jsb_CCStoreAssetsBuilder_constructor,
             __jsb_CCStoreAssetsBuilder_class,
             jsb_CCStoreAssetsBuilder_finalize)
SE_BIND_FUNC(jsb_CCStoreAssetsBuilder_setVersion)
SE_BIND_FUNC(jsb_CCStoreAssetsBuilder_addCurrency)
SE_BIND_FUNC(jsb_CCStoreAssetsBuilder_addGood)
SE_BIND_FUNC(jsb_CCStoreAssetsBuilder_addCurrencyPack)
SE_BIND_FUNC(jsb_CCStoreAssetsBuilder_addCategorie)
SE_BIND_FUNC(jsb_CCStoreAssetsBuilder_build)

bool register_cc_store_assets_builder_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCStoreAssetsBuilder", __soomlaObj, nullptr,
                                 _SE(jsb_CCStoreAssetsBuilder_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCStoreAssetsBuilder_finalize));

    cls->defineFunction("setVersion", _SE(jsb_CCStoreAssetsBuilder_setVersion));
    cls->defineFunction("addCurrency",
                        _SE(jsb_CCStoreAssetsBuilder_addCurrency));
    cls->defineFunction("addGood", _SE(jsb_CCStoreAssetsBuilder_addGood));
    cls->defineFunction("addCurrencyPack",
                        _SE(jsb_CCStoreAssetsBuilder_addCurrencyPack));
    cls->defineFunction("addCategorie",
                        _SE(jsb_CCStoreAssetsBuilder_addCategorie));
    cls->defineFunction("build", _SE(jsb_CCStoreAssetsBuilder_build));

    cls->install();

    JSBClassType::registerClass<CCStoreAssetsBuilder>(cls);

    __jsb_CCStoreAssetsBuilder_class = cls;

    //__jsb_CCStoreAssets_class
    auto clsStoreAssets =
        se::Class::create("CCStoreAssets", __soomlaObj, nullptr, nullptr);
    clsStoreAssets->install();
    JSBClassType::registerClass<CCStoreAssets>(clsStoreAssets);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
