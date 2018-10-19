//
//  jsb_cc_purchasable_virtual_item.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/18/18.
//

//#include "ee/jsb/soomla/jsb_cc_purchasable_virtual_item.hpp"

#include "Soomla/domain/CCPurchasableVirtualItem.h"
#include "Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/soomla/jsb_cc_virtual_item.hpp"

namespace soomla {
se::Object* __jsb_CCPurchasableVirtualItem_proto = nullptr;
se::Class* __jsb_CCPurchasableVirtualItem_class = nullptr;
} // namespace soomla

namespace ee {
namespace core {

template <>
void set_value(se::Value& value, soomla::CCPurchasableVirtualItem* input) {
    set_value_from_pointer(value, input);
}
} // namespace core
} // namespace ee

namespace soomla {

const auto jsb_CCPurchasableVirtualItem_finalize =
    &ee::core::jsb_finalize<CCPurchasableVirtualItem>;
const auto jsb_CCPurchasableVirtualItem_getPurchaseType =
    &ee::core::jsb_method_get<CCPurchasableVirtualItem,
                              &CCPurchasableVirtualItem::getPurchaseType,
                              CCPurchaseType*>;
const auto jsb_CCPurchasableVirtualItem_canAfford =
    &ee::core::jsb_method_get<CCPurchasableVirtualItem,
                              &CCPurchasableVirtualItem::canAfford, bool,
                              CCError**>;

SE_BIND_FINALIZE_FUNC(jsb_CCPurchasableVirtualItem_finalize)
SE_BIND_FUNC(jsb_CCPurchasableVirtualItem_getPurchaseType)
SE_BIND_FUNC(jsb_CCPurchasableVirtualItem_canAfford)

bool register_cc_purchasable_virtual_item_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCPurchasableVirtualItem", __soomlaObj,
                                 __jsb_CCVirtualItem_proto, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCPurchasableVirtualItem_finalize));
    cls->defineFunction("getPurchaseType",
                        _SE(jsb_CCPurchasableVirtualItem_getPurchaseType));
    cls->defineFunction("canAfford",
                        _SE(jsb_CCPurchasableVirtualItem_canAfford));

    cls->install();

    JSBClassType::registerClass<CCPurchasableVirtualItem>(cls);

    __jsb_CCPurchasableVirtualItem_proto = cls->getProto();
    __jsb_CCPurchasableVirtualItem_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
