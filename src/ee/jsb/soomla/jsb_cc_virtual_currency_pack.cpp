//
//  jsb_cc_virtual_item.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "ee/jsb/soomla/jsb_cc_virtual_currency_pack.hpp"

#include "Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"

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

const auto jsb_CCVirtualCurrencyPack_finalize =
    &ee::core::jsb_finalize<CCVirtualCurrencyPack>;
const auto jsb_CCVirtualCurrencyPack_getCurrencyAmount =
    &ee::core::jsb_method_get<CCVirtualCurrencyPack,
                              &CCVirtualCurrencyPack::getCurrencyAmount,
                              std::int32_t>;
const auto jsb_CCVirtualCurrencyPack_canAfford = &ee::core::jsb_method_get<
    CCVirtualCurrencyPack, &CCVirtualCurrencyPack::canAfford, bool, CCError**>;

SE_BIND_FINALIZE_FUNC(jsb_CCVirtualCurrencyPack_finalize)
SE_BIND_FUNC(jsb_CCVirtualCurrencyPack_getCurrencyAmount)
SE_BIND_FUNC(jsb_CCVirtualCurrencyPack_canAfford)

bool register_cc_virtual_currency_pack_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto purchasableVirtualItemProto = getCCPurchasableVirtualItemProto();
    auto cls = se::Class::create("CCVirtualCurrencyPack", __soomlaObj,
                                 purchasableVirtualItemProto, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCVirtualCurrencyPack_finalize));

    cls->defineFunction("getCurrencyAmount",
                        _SE(jsb_CCVirtualCurrencyPack_getCurrencyAmount));
    cls->defineFunction("canAfford", _SE(jsb_CCVirtualCurrencyPack_canAfford));

    cls->install();

    JSBClassType::registerClass<CCVirtualCurrencyPack>(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
