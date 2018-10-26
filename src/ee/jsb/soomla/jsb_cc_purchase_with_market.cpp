//
//  jsb_cc_purchase_with_market.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/18/18.
//

#include "ee/jsb/soomla/jsb_cc_purchase_with_market.hpp"

#include "Soomla/PurchaseTypes/CCPurchaseWithMarket.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {

template <>
void set_value(se::Value& value, soomla::CCPurchaseWithMarket* input) {
    set_value_from_pointer(value, input);
}

template <>
void set_value(se::Value& value, soomla::CCMarketItem* input) {
    set_value_from_pointer(value, input);
}
} // namespace core
} // namespace ee

namespace soomla {
se::Class* __jsb_CCPurchaseWithMarket_class = nullptr;

const auto jsb_CCPurchaseWithMarket_finalize =
    &ee::core::jsb_finalize<CCPurchaseWithMarket>;
const auto jsb_CCPurchaseWithMarket_constructor =
    &ee::core::jsb_constructor<CCPurchaseWithMarket>;
const auto jsb_CCPurchaseWithMarket_getMarketItem = &ee::core::jsb_method_get<
    CCPurchaseWithMarket, &CCPurchaseWithMarket::getMarketItem, CCMarketItem*>;

SE_BIND_FINALIZE_FUNC(jsb_CCPurchaseWithMarket_finalize)
SE_BIND_CTOR(jsb_CCPurchaseWithMarket_constructor,
             __jsb_CCPurchaseWithMarket_class,
             jsb_CCPurchaseWithMarket_finalize)
SE_BIND_FUNC(jsb_CCPurchaseWithMarket_getMarketItem)

bool register_cc_purchase_with_market_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCPurchaseWithMarket", __soomlaObj, nullptr,
                                 _SE(jsb_CCPurchaseWithMarket_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCPurchaseWithMarket_finalize));

    cls->defineFunction("getMarketItem",
                        _SE(jsb_CCPurchaseWithMarket_getMarketItem));

    cls->install();

    JSBClassType::registerClass<CCPurchaseWithMarket>(cls);

    __jsb_CCPurchaseWithMarket_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
