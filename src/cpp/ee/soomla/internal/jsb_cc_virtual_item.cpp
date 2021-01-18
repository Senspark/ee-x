//
//  jsb_cc_virtual_item.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "ee/soomla/internal/jsb_cc_virtual_item.hpp"

#include <ee/core/JsbUtils.hpp>
#include <soomla/domain/CCPurchasableVirtualItem.h>
#include <soomla/domain/CCVirtualItem.h>
#include <soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h>

#include "ee/soomla/internal/jsb_cc_purchasable_virtual_item.hpp"

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, soomla::CCVirtualItem* input) {
    set_value_from_pointer(value, input);
}

template <>
soomla::CCVirtualItem* get_value(const se::Value& value) {
    return static_cast<soomla::CCVirtualItem*>(
        value.toObject()->getPrivateData());
}
} // namespace core
} // namespace ee

namespace soomla {
se::Object* __jsb_CCVirtualItem_proto = nullptr;

CCPurchasableVirtualItem* asPurchasableVirtualItem(CCVirtualItem* item) {
    return dynamic_cast<CCPurchasableVirtualItem*>(item);
}

const auto jsb_CCVirtualItem_finalize = &ee::core::jsb_finalize<CCVirtualItem>;
const auto jsb_CCVirtualItem_getItemId =
    &ee::core::jsb_method_get<CCVirtualItem, &CCVirtualItem::getItemId,
                              const std::string&>;
const auto jsb_CCVirtualItem_give =
    &ee::core::jsb_method_get<CCVirtualItem,
                              (int (CCVirtualItem::*)(int, CCError**)) &
                                  CCVirtualItem::give,
                              int, int, CCError**>;
const auto jsb_CCVirtualItem_take =
    &ee::core::jsb_method_get<CCVirtualItem,
                              (int (CCVirtualItem::*)(int, CCError**)) &
                                  CCVirtualItem::take,
                              int, int, CCError**>;
const auto jsb_CCVirtualItem_resetBalance =
    &ee::core::jsb_method_get<CCVirtualItem,
                              (int (CCVirtualItem::*)(int, CCError**)) &
                                  CCVirtualItem::resetBalance,
                              int, int, CCError**>;
const auto jsb_CCVirtualItem_getBalance = &ee::core::jsb_method_get<
    CCVirtualItem, (int (CCVirtualItem::*)()) & CCVirtualItem::getBalance, int>;
const auto jsb_CCVirtualItem_save =
    &ee::core::jsb_method_call<CCVirtualItem, &CCVirtualItem::save, bool>;

SE_BIND_FINALIZE_FUNC(jsb_CCVirtualItem_finalize)
SE_BIND_FUNC(jsb_CCVirtualItem_getItemId)
SE_BIND_FUNC(jsb_CCVirtualItem_give)
SE_BIND_FUNC(jsb_CCVirtualItem_take)
SE_BIND_FUNC(jsb_CCVirtualItem_resetBalance)
SE_BIND_FUNC(jsb_CCVirtualItem_getBalance)
SE_BIND_FUNC(jsb_CCVirtualItem_save)

se::Object* getCCVirtualItemProto() {
    CCASSERT(__jsb_CCVirtualItem_proto != nullptr,
             "__jsb_CCVirtualItem_proto null");
    return __jsb_CCVirtualItem_proto;
}

bool register_cc_virtual_item_manual(se::Object* globalObj) {
    auto __soomlaObj = ee::JsbUtils::getPath(globalObj, "soomla");

    auto cls =
        se::Class::create("CCVirtualItem", __soomlaObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCVirtualItem_finalize));

    cls->defineFunction("getItemId", _SE(jsb_CCVirtualItem_getItemId));
    cls->defineFunction("give", _SE(jsb_CCVirtualItem_give));
    cls->defineFunction("take", _SE(jsb_CCVirtualItem_take));
    cls->defineFunction("resetBalance", _SE(jsb_CCVirtualItem_resetBalance));
    cls->defineFunction("getBalance", _SE(jsb_CCVirtualItem_getBalance));
    cls->defineFunction("save", _SE(jsb_CCVirtualItem_save));

    cls->install();

    JSBClassType::registerClass<CCVirtualItem>(cls);

    __jsb_CCVirtualItem_proto = cls->getProto();

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
