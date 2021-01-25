//
//  jsb_cc_purchase_with_market_builder.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "ee/soomla/internal/jsb_cc_purchase_with_market_builder.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>
#include <soomla/PurchaseTypes/CCPurchaseWithMarket.h>
#include <soomla/PurchaseTypes/CCPurchaseWithMarketBuilder.h>

namespace ee {
namespace core {
template <>
soomla::CCPurchaseType* get_value(const se::Value& value) {
    return static_cast<soomla::CCPurchaseType*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, soomla::CCPurchaseType* input) {
    set_value_from_pointer(value, input);
}

template <>
void set_value(se::Value& value, soomla::CCPurchaseWithMarketBuilder& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core
} // namespace ee

namespace soomla {
namespace {
se::Class* clazz = nullptr;

using Self = CCPurchaseWithMarketBuilder;

// clang-format off
constexpr auto finalize     = &ee::core::makeFinalize<Self>;
constexpr auto constructor  = &ee::core::makeConstructor<Self>;
constexpr auto setPrice     = &ee::core::makeInstanceMethod<&Self::setPrice>;
constexpr auto setProductId = &ee::core::makeInstanceMethod<&Self::setProductId>;
constexpr auto build        = &ee::core::makeInstanceMethod<&Self::build>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(setPrice)
SE_BIND_FUNC(setProductId)
SE_BIND_FUNC(build)

constexpr auto jsb_CCPurchaseType_finalize =
    &ee::core::makeFinalize<CCPurchaseType>;

SE_BIND_FINALIZE_FUNC(jsb_CCPurchaseType_finalize)
} // namespace

bool register_cc_purchase_with_market_builder_manual(se::Object* globalObj) {
    auto __soomlaObj = ee::JsbUtils::getPath(globalObj, "soomla");

    auto cls = se::Class::create("CCPurchaseWithMarketBuilder", __soomlaObj,
                                 nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setPrice);
    EE_JSB_DEFINE_FUNCTION(cls, setProductId);
    EE_JSB_DEFINE_FUNCTION(cls, build);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    auto clsPurchase =
        se::Class::create("CCPurchaseType", __soomlaObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCPurchaseType_finalize));

    clsPurchase->install();
    JSBClassType::registerClass<CCPurchaseType>(clsPurchase);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla

#endif // EE_X_COCOS_JS