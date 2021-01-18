//
//  jsb_cc_virtual_currency_pack_builder.cpp
//  ee-x
//
//  Created by Kiet Le on 9/19/18.
//

#include "ee/soomla/internal/jsb_cc_virtual_currency_pack_builder.hpp"

#include <ee/core/JsbUtils.hpp>
#include <soomla/domain/virtualCurrencies/CCVirtualCurrencyBuilder.h>

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, soomla::CCVirtualCurrencyBuilder& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core
} // namespace ee

namespace soomla {
namespace {
se::Class* clazz = nullptr;

using Self = CCVirtualCurrencyBuilder;

// clang-format off
constexpr auto finalize       = &ee::core::makeFinalize<Self>;
constexpr auto constructor    = &ee::core::makeConstructor<Self>;
constexpr auto setName        = &ee::core::makeInstanceMethod<&Self::setName>;
constexpr auto setDescription = &ee::core::makeInstanceMethod<&Self::setDescription>;
constexpr auto setItemId      = &ee::core::makeInstanceMethod<&Self::setItemId>;
constexpr auto build          = &ee::core::makeInstanceMethod<&Self::build>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(setName)
SE_BIND_FUNC(setDescription)
SE_BIND_FUNC(setItemId)
SE_BIND_FUNC(build)
} // namespace

bool register_cc_virtual_currency_builder_manual(se::Object* globalObj) {
    auto __soomlaObj = ee::JsbUtils::getPath(globalObj, "soomla");

    auto cls = se::Class::create("CCVirtualCurrencyBuilder", __soomlaObj,
                                 nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setName);
    EE_JSB_DEFINE_FUNCTION(cls, setDescription);
    EE_JSB_DEFINE_FUNCTION(cls, setItemId);
    EE_JSB_DEFINE_FUNCTION(cls, build);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
