//
//  jsb_cc_store_assets_builder.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/13/18.
//

#include "ee/jsb/soomla/jsb_cc_store_assets_builder.hpp"

#include <Soomla/CCStoreAssetsBuilder.h>
#include <Soomla/Cocos2dxCore.h>

#include <ee/jsb/JsbCore.hpp>

namespace soomla {
namespace {
se::Class* clazz = nullptr;
} // namespace
} // namespace soomla

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
namespace {
using Self = CCStoreAssetsBuilder;

// clang-format off
constexpr auto constructor     = &ee::core::makeConstructor<Self>;
constexpr auto finalize        = &ee::core::makeFinalize<Self>;
constexpr auto setVersion      = &ee::core::makeInstanceMethod<&Self::setVersion>;
constexpr auto addCurrency     = &ee::core::makeInstanceMethod<&Self::addCurrency>;
constexpr auto addGood         = &ee::core::makeInstanceMethod<&Self::addGood>;
constexpr auto addCurrencyPack = &ee::core::makeInstanceMethod<&Self::addCurrencyPack>;
constexpr auto addCategory     = &ee::core::makeInstanceMethod<&Self::addCategorie>;
constexpr auto build           = &ee::core::makeInstanceMethod<&Self::build>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(setVersion)
SE_BIND_FUNC(addCurrency)
SE_BIND_FUNC(addGood)
SE_BIND_FUNC(addCurrencyPack)
SE_BIND_FUNC(addCategory)
SE_BIND_FUNC(build)
} // namespace

bool register_cc_store_assets_builder_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCStoreAssetsBuilder", __soomlaObj, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setVersion);
    EE_JSB_DEFINE_FUNCTION(cls, addCurrency);
    EE_JSB_DEFINE_FUNCTION(cls, addGood);
    EE_JSB_DEFINE_FUNCTION(cls, addCurrencyPack);
    EE_JSB_DEFINE_FUNCTION(cls, addCategory);
    EE_JSB_DEFINE_FUNCTION(cls, build);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    //__jsb_CCStoreAssets_class
    auto clsStoreAssets =
        se::Class::create("CCStoreAssets", __soomlaObj, nullptr, nullptr);
    clsStoreAssets->install();
    JSBClassType::registerClass<CCStoreAssets>(clsStoreAssets);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
