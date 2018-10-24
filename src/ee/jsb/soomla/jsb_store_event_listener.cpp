//
//  jsb_store_event_listener.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/17/18.
//

#include "ee/jsb/soomla/jsb_store_event_listener.hpp"

#include "Soomla/StoreEventListener.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/soomla/jsb_cc_soomla.hpp"

namespace soomla {

se::Object* __jsb_StoreEventListener_proto = nullptr;
se::Class* __jsb_StoreEventListener_class = nullptr;

const auto jsb_StoreEventListener_finalize =
    &ee::core::jsb_finalize<StoreEventListener>;
const auto jsb_StoreEventListener_constructor =
    &ee::core::jsb_constructor<StoreEventListener>;
const auto jsb_StoreEventListener_clear =
    &ee::core::jsb_method_call<StoreEventListener, &StoreEventListener::clear>;
const auto jsb_StoreEventListener_setMarketPurchaseStartedCallback =
    &ee::core::jsb_set_callback<
        StoreEventListener,
        &StoreEventListener::setMarketPurchaseStartedCallback,
        const std::string&>;
const auto jsb_StoreEventListener_setMarketPurchaseCanceledCallback =
    &ee::core::jsb_set_callback<
        StoreEventListener,
        &StoreEventListener::setMarketPurchaseCanceledCallback,
        const std::string&>;
const auto jsb_StoreEventListener_setCurrencyBalanceChangedCallback =
    &ee::core::jsb_set_callback<
        StoreEventListener,
        &StoreEventListener::setCurrencyBalanceChangedCallback,
        const std::string&, int, int>;
const auto jsb_StoreEventListener_setUnexpectedStoreErrorCallback =
    &ee::core::jsb_set_callback<
        StoreEventListener,
        &StoreEventListener::setUnexpectedStoreErrorCallback, int>;
const auto jsb_StoreEventListener_setGoodUpgradeCallback =
    &ee::core::jsb_set_callback<StoreEventListener,
                                &StoreEventListener::setGoodUpgradeCallback,
                                const std::string&, const std::string&>;
const auto jsb_StoreEventListener_setItemPurchasedCallback =
    &ee::core::jsb_set_callback<StoreEventListener,
                                &StoreEventListener::setItemPurchasedCallback,
                                const std::string&, const std::string&>;
const auto jsb_StoreEventListener_setMarketPurchaseCallback =
    &ee::core::jsb_set_callback<StoreEventListener,
                                &StoreEventListener::setMarketPurchaseCallback,
                                const std::string&, const std::string&,
                                std::unordered_map<std::string, std::string>>;
const auto jsb_StoreEventListener_setMarketItemsRefreshStartedCallback =
    &ee::core::jsb_set_callback<
        StoreEventListener,
        &StoreEventListener::setMarketItemsRefreshStartedCallback>;

const auto jsb_StoreEventListener_setMarketItemsRefreshedCallback =
    &ee::core::jsb_set_callback<
        StoreEventListener,
        &StoreEventListener::setMarketItemsRefreshedCallback,
        std::vector<std::string>>;

const auto jsb_StoreEventListener_setMarketItemsRefreshFailedCallback =
    &ee::core::jsb_set_callback<
        StoreEventListener,
        &StoreEventListener::setMarketItemsRefreshFailedCallback,
        const std::string&>;
const auto jsb_StoreEventListener_setRestoreTransactionStartedCallback =
    &ee::core::jsb_set_callback<
        StoreEventListener,
        &StoreEventListener::setRestoreTransactionStartedCallback>;
const auto jsb_StoreEventListener_setRestoreTransactionFinishedCallback =
    &ee::core::jsb_set_callback<
        StoreEventListener,
        &StoreEventListener::setRestoreTransactionFinishedCallback, bool>;

SE_BIND_FINALIZE_FUNC(jsb_StoreEventListener_finalize)
SE_BIND_CTOR(jsb_StoreEventListener_constructor, __jsb_StoreEventListener_class,
             jsb_StoreEventListener_finalize)
SE_BIND_FUNC(jsb_StoreEventListener_clear)
SE_BIND_FUNC(jsb_StoreEventListener_setMarketPurchaseStartedCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setMarketPurchaseCanceledCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setCurrencyBalanceChangedCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setUnexpectedStoreErrorCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setGoodUpgradeCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setItemPurchasedCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setMarketPurchaseCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setMarketItemsRefreshStartedCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setMarketItemsRefreshedCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setMarketItemsRefreshFailedCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setRestoreTransactionStartedCallback)
SE_BIND_FUNC(jsb_StoreEventListener_setRestoreTransactionFinishedCallback)

bool register_store_event_listener_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("StoreEventListener", __soomlaObj, nullptr,
                                 _SE(jsb_StoreEventListener_constructor));

    cls->defineFinalizeFunction(_SE(jsb_StoreEventListener_finalize));
    cls->defineFunction("clear", _SE(jsb_StoreEventListener_clear));
    cls->defineFunction(
        "setMarketPurchaseStartedCallback",
        _SE(jsb_StoreEventListener_setMarketPurchaseStartedCallback));

    cls->defineFunction(
        "setMarketPurchaseCanceledCallback",
        _SE(jsb_StoreEventListener_setMarketPurchaseCanceledCallback));

    cls->defineFunction("setGoodUpgradeCallback",
                        _SE(jsb_StoreEventListener_setGoodUpgradeCallback));

    cls->defineFunction("setItemPurchasedCallback",
                        _SE(jsb_StoreEventListener_setItemPurchasedCallback));

    cls->defineFunction("setMarketPurchaseCallback",
                        _SE(jsb_StoreEventListener_setMarketPurchaseCallback));

    cls->defineFunction(
        "setMarketItemsRefreshStartedCallback",
        _SE(jsb_StoreEventListener_setMarketItemsRefreshStartedCallback));

    cls->defineFunction(
        "setMarketItemsRefreshedCallback",
        _SE(jsb_StoreEventListener_setMarketItemsRefreshedCallback));

    cls->defineFunction(
        "setMarketItemsRefreshFailedCallback",
        _SE(jsb_StoreEventListener_setMarketItemsRefreshFailedCallback));

    cls->defineFunction(
        "setRestoreTransactionStartedCallback",
        _SE(jsb_StoreEventListener_setRestoreTransactionStartedCallback));

    cls->defineFunction(
        "setRestoreTransactionFinishedCallback",
        _SE(jsb_StoreEventListener_setRestoreTransactionFinishedCallback));

    cls->defineFunction(
        "setUnexpectedStoreErrorCallback",
        _SE(jsb_StoreEventListener_setUnexpectedStoreErrorCallback));

    cls->defineFunction(
        "setCurrencyBalanceChangedCallback",
        _SE(jsb_StoreEventListener_setCurrencyBalanceChangedCallback));

    cls->install();

    JSBClassType::registerClass<StoreEventListener>(cls);

    __jsb_StoreEventListener_proto = cls->getProto();
    __jsb_StoreEventListener_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
