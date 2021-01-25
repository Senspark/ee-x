//
//  jsb_store_event_listener.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/17/18.
//

#include "ee/soomla/internal/jsb_store_event_listener.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>
#include <soomla/StoreEventListener.h>

namespace soomla {
namespace {
se::Class* clazz = nullptr;

using Self = StoreEventListener;

// clang-format off
constexpr auto constructor                           = &ee::JsbUtils::makeConstructor<Self>;
constexpr auto finalize                              = &ee::JsbUtils::makeFinalize<Self>;
constexpr auto clear                                 = &ee::JsbUtils::makeMethod<&Self::clear>;
constexpr auto setMarketPurchaseStartedCallback      = &ee::JsbUtils::makeMethod<&Self::setMarketPurchaseStartedCallback>;
constexpr auto setMarketPurchaseCanceledCallback     = &ee::JsbUtils::makeMethod<&Self::setMarketPurchaseCanceledCallback>;
constexpr auto setCurrencyBalanceChangedCallback     = &ee::JsbUtils::makeMethod<&Self::setCurrencyBalanceChangedCallback>;
constexpr auto setUnexpectedStoreErrorCallback       = &ee::JsbUtils::makeMethod<&Self::setUnexpectedStoreErrorCallback>;
constexpr auto setGoodUpgradeCallback                = &ee::JsbUtils::makeMethod<&Self::setGoodUpgradeCallback>;
constexpr auto setItemPurchasedCallback              = &ee::JsbUtils::makeMethod<&Self::setItemPurchasedCallback>;
constexpr auto setMarketPurchaseCallback             = &ee::JsbUtils::makeMethod<&Self::setMarketPurchaseCallback>;
constexpr auto setMarketItemsRefreshStartedCallback  = &ee::JsbUtils::makeMethod<&Self::setMarketItemsRefreshStartedCallback>;
constexpr auto setMarketItemsRefreshedCallback       = &ee::JsbUtils::makeMethod<&Self::setMarketItemsRefreshedCallback>;
constexpr auto setMarketItemsRefreshFailedCallback   = &ee::JsbUtils::makeMethod<&Self::setMarketItemsRefreshFailedCallback>;
constexpr auto setRestoreTransactionStartedCallback  = &ee::JsbUtils::makeMethod<&Self::setRestoreTransactionStartedCallback>;
constexpr auto setRestoreTransactionFinishedCallback = &ee::JsbUtils::makeMethod<&Self::setRestoreTransactionFinishedCallback>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(clear)
SE_BIND_FUNC(setMarketPurchaseStartedCallback)
SE_BIND_FUNC(setMarketPurchaseCanceledCallback)
SE_BIND_FUNC(setCurrencyBalanceChangedCallback)
SE_BIND_FUNC(setUnexpectedStoreErrorCallback)
SE_BIND_FUNC(setGoodUpgradeCallback)
SE_BIND_FUNC(setItemPurchasedCallback)
SE_BIND_FUNC(setMarketPurchaseCallback)
SE_BIND_FUNC(setMarketItemsRefreshStartedCallback)
SE_BIND_FUNC(setMarketItemsRefreshedCallback)
SE_BIND_FUNC(setMarketItemsRefreshFailedCallback)
SE_BIND_FUNC(setRestoreTransactionStartedCallback)
SE_BIND_FUNC(setRestoreTransactionFinishedCallback)
} // namespace

bool register_store_event_listener_manual(se::Object* globalObj) {
    auto __soomlaObj = ee::JsbUtils::getPath(globalObj, "soomla");

    auto cls = se::Class::create("StoreEventListener", __soomlaObj, nullptr,
                                 _SE(constructor));

    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, clear);
    EE_JSB_DEFINE_FUNCTION(cls, setMarketPurchaseStartedCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setMarketPurchaseCanceledCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setCurrencyBalanceChangedCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setUnexpectedStoreErrorCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setGoodUpgradeCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setItemPurchasedCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setMarketPurchaseCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setMarketItemsRefreshStartedCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setMarketItemsRefreshedCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setMarketItemsRefreshFailedCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setRestoreTransactionStartedCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setRestoreTransactionFinishedCallback);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla

#endif // EE_X_COCOS_JS