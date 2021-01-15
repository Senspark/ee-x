//
//  jsb_store_event_listener.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/17/18.
//

#include "ee/jsb/soomla/jsb_store_event_listener.hpp"

#include <Soomla/StoreEventListener.h>

#include <ee/jsb/JsbCore.hpp>

namespace soomla {
namespace {
se::Class* clazz = nullptr;

using Self = StoreEventListener;

// clang-format off
constexpr auto constructor                           = &ee::core::makeConstructor<Self>;
constexpr auto finalize                              = &ee::core::makeFinalize<Self>;
constexpr auto clear                                 = &ee::core::makeInstanceMethod<&Self::clear>;
constexpr auto setMarketPurchaseStartedCallback      = &ee::core::makeInstanceMethod<&Self::setMarketPurchaseStartedCallback>;
constexpr auto setMarketPurchaseCanceledCallback     = &ee::core::makeInstanceMethod<&Self::setMarketPurchaseCanceledCallback>;
constexpr auto setCurrencyBalanceChangedCallback     = &ee::core::makeInstanceMethod<&Self::setCurrencyBalanceChangedCallback>;
constexpr auto setUnexpectedStoreErrorCallback       = &ee::core::makeInstanceMethod<&Self::setUnexpectedStoreErrorCallback>;
constexpr auto setGoodUpgradeCallback                = &ee::core::makeInstanceMethod<&Self::setGoodUpgradeCallback>;
constexpr auto setItemPurchasedCallback              = &ee::core::makeInstanceMethod<&Self::setItemPurchasedCallback>;
constexpr auto setMarketPurchaseCallback             = &ee::core::makeInstanceMethod<&Self::setMarketPurchaseCallback>;
constexpr auto setMarketItemsRefreshStartedCallback  = &ee::core::makeInstanceMethod<&Self::setMarketItemsRefreshStartedCallback>;
constexpr auto setMarketItemsRefreshedCallback       = &ee::core::makeInstanceMethod<&Self::setMarketItemsRefreshedCallback>;
constexpr auto setMarketItemsRefreshFailedCallback   = &ee::core::makeInstanceMethod<&Self::setMarketItemsRefreshFailedCallback>;
constexpr auto setRestoreTransactionStartedCallback  = &ee::core::makeInstanceMethod<&Self::setRestoreTransactionStartedCallback>;
constexpr auto setRestoreTransactionFinishedCallback = &ee::core::makeInstanceMethod<&Self::setRestoreTransactionFinishedCallback>;
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
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

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
