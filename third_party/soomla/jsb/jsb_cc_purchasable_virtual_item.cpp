//
//  jsb_cc_purchasable_virtual_item.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/18/18.
//

#include "ee/jsb/soomla/jsb_cc_purchasable_virtual_item.hpp"

#include <Soomla/domain/CCPurchasableVirtualItem.h>
#include <Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h>

#include <ee/jsb/JsbCore.hpp>

#include "ee/jsb/soomla/jsb_cc_virtual_item.hpp"

namespace ee {
namespace core {

template <>
void set_value(se::Value& value, soomla::CCPurchasableVirtualItem* input) {
    set_value_from_pointer(value, input);
}
} // namespace core
} // namespace ee

namespace soomla {
se::Object* __jsb_CCPurchasableVirtualItem_proto = nullptr;

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

se::Object* getCCPurchasableVirtualItemProto() {
    CCASSERT(__jsb_CCPurchasableVirtualItem_proto != nullptr,
             "__jsb_CCPurchasableVirtualItem_proto null");
    return __jsb_CCPurchasableVirtualItem_proto;
}

bool register_cc_purchasable_virtual_item_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto virtualItemProto = getCCVirtualItemProto();
    auto cls = se::Class::create("CCPurchasableVirtualItem", __soomlaObj,
                                 virtualItemProto, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCPurchasableVirtualItem_finalize));
    cls->defineFunction("getPurchaseType",
                        _SE(jsb_CCPurchasableVirtualItem_getPurchaseType));
    cls->defineFunction("canAfford",
                        _SE(jsb_CCPurchasableVirtualItem_canAfford));

    cls->install();

    JSBClassType::registerClass<CCPurchasableVirtualItem>(cls);

    __jsb_CCPurchasableVirtualItem_proto = cls->getProto();

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
