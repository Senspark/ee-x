//
//  jsb_cc_virtual_item.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "ee/jsb/soomla/jsb_cc_virtual_currency_pack.hpp"

#include <Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/soomla/jsb_cc_purchasable_virtual_item.hpp"

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, soomla::CCVirtualCurrencyPack* input) {
    set_value_from_pointer(value, input);
}
} // namespace core
} // namespace ee

namespace soomla {
using Self = CCVirtualCurrencyPack;

// clang-format off
constexpr auto finalize          = &ee::core::makeFinalize<Self>;
constexpr auto getCurrencyAmount = &ee::core::makeInstanceMethod<&Self::getCurrencyAmount>;
constexpr auto canAfford         = &ee::core::makeInstanceMethod<&Self::canAfford>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(getCurrencyAmount)
SE_BIND_FUNC(canAfford)

bool register_cc_virtual_currency_pack_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto purchasableVirtualItemProto = getCCPurchasableVirtualItemProto();
    auto cls = se::Class::create("CCVirtualCurrencyPack", __soomlaObj,
                                 purchasableVirtualItemProto, nullptr);

    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, getCurrencyAmount);
    EE_JSB_DEFINE_FUNCTION(cls, canAfford);

    cls->install();
    JSBClassType::registerClass<Self>(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
