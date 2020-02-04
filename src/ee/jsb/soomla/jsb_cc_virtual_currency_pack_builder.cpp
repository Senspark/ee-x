//
//  jsb_cc_virtual_currency_pack_builder.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "ee/jsb/soomla/jsb_cc_virtual_currency_pack_builder.hpp"

#include <Soomla/domain/virtualCurrencies/CCVirtualCurrencyPackBuilder.h>

#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, soomla::CCVirtualCurrencyPackBuilder& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core
} // namespace ee

namespace soomla {
namespace {
se::Class* clazz = nullptr;

using Self = CCVirtualCurrencyPackBuilder;

// clang-format off
constexpr auto constructor       = &ee::core::makeConstructor<Self>;
constexpr auto finalize          = &ee::core::makeFinalize<Self>;
constexpr auto setName           = &ee::core::makeInstanceMethod<&Self::setName>;
constexpr auto setDescription    = &ee::core::makeInstanceMethod<&Self::setDescription>;
constexpr auto setItemId         = &ee::core::makeInstanceMethod<&Self::setItemId>;
constexpr auto setCurrencyAmount = &ee::core::makeInstanceMethod<&Self::setCurrencyAmount>;
constexpr auto setCurrencyItemId = &ee::core::makeInstanceMethod<&Self::setCurrencyItemId>;
constexpr auto setPurchaseType   = &ee::core::makeInstanceMethod<&Self::setPurchaseType>;
constexpr auto build             = &ee::core::makeInstanceMethod<&Self::build>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(setName)
SE_BIND_FUNC(setDescription)
SE_BIND_FUNC(setItemId)
SE_BIND_FUNC(setCurrencyAmount)
SE_BIND_FUNC(setCurrencyItemId)
SE_BIND_FUNC(setPurchaseType)
SE_BIND_FUNC(build)
} // namespace

bool register_cc_virtual_currency_pack_builder_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCVirtualCurrencyPackBuilder", __soomlaObj,
                                 nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setName);
    EE_JSB_DEFINE_FUNCTION(cls, setDescription);
    EE_JSB_DEFINE_FUNCTION(cls, setItemId);
    EE_JSB_DEFINE_FUNCTION(cls, setCurrencyAmount);
    EE_JSB_DEFINE_FUNCTION(cls, setCurrencyItemId);
    EE_JSB_DEFINE_FUNCTION(cls, setPurchaseType);
    EE_JSB_DEFINE_FUNCTION(cls, build);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
