//
//  jsb_cc_market_item.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/19/18.
//

#include "ee/soomla/internal/jsb_cc_market_item.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>
#include <soomla/domain/CCMarketItem.h>

namespace soomla {

se::Class* __jsb_CCMarketItem_class = nullptr;

const auto jsb_CCMarketItem_finalize =
    &ee::JsbUtils::makeFinalize<CCMarketItem>;
const auto jsb_CCMarketItem_constructor =
    &ee::core::jsb_constructor<CCMarketItem>;
const auto jsb_CCMarketItem_getMarketPriceAndCurrency =
    &ee::core::jsb_method_get<CCMarketItem,
                              &CCMarketItem::getMarketPriceAndCurrency,
                              const std::string&>;
const auto jsb_CCMarketItem_getMarketCurrencyCode =
        &ee::core::jsb_method_get<CCMarketItem,
                &CCMarketItem::getMarketCurrencyCode,
                const std::string&>;
const auto jsb_CCMarketItem_getProductId =
    &ee::core::jsb_method_get<CCMarketItem, &CCMarketItem::getProductId,
                              const std::string&>;
const auto jsb_CCMarketItem_getPrice =
    &ee::core::jsb_method_get<CCMarketItem, &CCMarketItem::getPrice, double>;
const auto jsb_CCMarketItem_getMarketTitle =
    &ee::core::jsb_method_get<CCMarketItem, &CCMarketItem::getMarketTitle,
                              const std::string&>;
const auto jsb_CCMarketItem_getMarketPriceMicros =
    &ee::core::jsb_method_get<CCMarketItem, &CCMarketItem::getMarketPriceMicros,
                              double>;

SE_BIND_FINALIZE_FUNC(jsb_CCMarketItem_finalize)
SE_BIND_CTOR(jsb_CCMarketItem_constructor, __jsb_CCMarketItem_class,
             jsb_CCMarketItem_finalize)
SE_BIND_FUNC(jsb_CCMarketItem_getMarketPriceAndCurrency)
SE_BIND_FUNC(jsb_CCMarketItem_getMarketCurrencyCode)
SE_BIND_FUNC(jsb_CCMarketItem_getProductId)
SE_BIND_FUNC(jsb_CCMarketItem_getPrice)
SE_BIND_FUNC(jsb_CCMarketItem_getMarketTitle)
SE_BIND_FUNC(jsb_CCMarketItem_getMarketPriceMicros);

bool register_cc_market_item_manual(se::Object* globalObj) {
    auto __soomlaObj = ee::JsbUtils::getPath(globalObj, "soomla");

    auto cls = se::Class::create("CCMarketItem", __soomlaObj, nullptr,
                                 _SE(jsb_CCMarketItem_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCMarketItem_finalize));

    cls->defineFunction("getMarketPriceAndCurrency",
                        _SE(jsb_CCMarketItem_getMarketPriceAndCurrency));
    cls->defineFunction("getMarketCurrencyCode",
                        _SE(jsb_CCMarketItem_getMarketCurrencyCode));
    cls->defineFunction("getProductId", _SE(jsb_CCMarketItem_getProductId));
    cls->defineFunction("getPrice", _SE(jsb_CCMarketItem_getPrice));
    cls->defineFunction("getMarketTitle", _SE(jsb_CCMarketItem_getMarketTitle));
    cls->defineFunction("getMarketPriceMicros",
                        _SE(jsb_CCMarketItem_getMarketPriceMicros));

    cls->install();

    JSBClassType::registerClass<CCMarketItem>(cls);

    __jsb_CCMarketItem_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla

#endif // EE_X_COCOS_JS