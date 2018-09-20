//
//  jsb_cc_purchase_with_market.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/18/18.
//

#include "CCPurchaseWithMarket.h"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_cc_purchase_with_market.hpp"
#include "jsb_core_common.hpp"

namespace soomla {
static se::Object* __jsb_CCPurchaseWithMarket_proto = nullptr;
static se::Class* __jsb_CCPurchaseWithMarket_class = nullptr;
} // namespace soomla

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

static se::Object* __soomlaObj = nullptr;

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
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCPurchaseWithMarket", __soomlaObj, nullptr,
                                 _SE(jsb_CCPurchaseWithMarket_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCPurchaseWithMarket_finalize));

    cls->defineFunction("getMarketItem",
                        _SE(jsb_CCPurchaseWithMarket_getMarketItem));

    cls->install();

    JSBClassType::registerClass<CCPurchaseWithMarket>(cls);

    __jsb_CCPurchaseWithMarket_proto = cls->getProto();
    __jsb_CCPurchaseWithMarket_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
