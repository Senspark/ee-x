//
//  jsb_cc_market_item.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/19/18.
//

#include "CCMarketItem.h"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_cc_market_item.hpp"
#include "jsb_core_common.hpp"

namespace ee {
namespace core {}
} // namespace ee

namespace soomla {

static se::Object* __jsb_CCMarketItem_proto = nullptr;
static se::Class* __jsb_CCMarketItem_class = nullptr;
static se::Object* __soomlaObj = nullptr;

const auto jsb_CCMarketItem_finalize = &ee::core::jsb_finalize<CCMarketItem>;
const auto jsb_CCMarketItem_constructor =
    &ee::core::jsb_constructor<CCMarketItem>;
const auto jsb_CCMarketItem_getMarketPriceAndCurrency =
    &ee::core::jsb_method_get<
        CCMarketItem, &CCMarketItem::getMarketPriceAndCurrency, std::string>;

SE_BIND_FINALIZE_FUNC(jsb_CCMarketItem_finalize)
SE_BIND_CTOR(jsb_CCMarketItem_constructor, __jsb_CCMarketItem_class,
             jsb_CCMarketItem_finalize)
SE_BIND_FUNC(jsb_CCMarketItem_getMarketPriceAndCurrency)

bool register_cc_market_item_manual(se::Object* globalObj) {
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCMarketItem", __soomlaObj, nullptr,
                                 _SE(jsb_CCMarketItem_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCMarketItem_finalize));

    cls->defineFunction("getMarketPriceAndCurrency",
                        _SE(jsb_CCMarketItem_getMarketPriceAndCurrency));

    cls->install();

    JSBClassType::registerClass<CCMarketItem>(cls);

    __jsb_CCMarketItem_proto = cls->getProto();
    __jsb_CCMarketItem_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
