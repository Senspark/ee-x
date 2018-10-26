//
//  jsb_cc_soomla_store.cpp
//  ee-x
//
//  Created by Kiet Le on 9/13/18.
//

#include "ee/jsb/soomla/jsb_cc_soomla_store.hpp"

#include "Soomla/Cocos2dxStore.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {

template <>
soomla::CCStoreAssets* get_value(const se::Value& value) {
    return static_cast<soomla::CCStoreAssets*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, soomla::CCSoomlaStore* input) {
    set_value_from_pointer(value, input);
}

} // namespace core
} // namespace ee

namespace soomla {

const auto jsb_CCSoomlaStore_initialize =
    &ee::core::jsb_static_call<&CCSoomlaStore::initialize, CCStoreAssets*,
                               cocos2d::ValueMap>;
const auto jsb_CCSoomlaStore_getInstance =
    &ee::core::jsb_static_get<CCSoomlaStore*, &CCSoomlaStore::getInstance>;
const auto jsb_CCSoomlaStore_refreshMarketItemsDetails =
    &ee::core::jsb_method_call<
        CCSoomlaStore, &CCSoomlaStore::refreshMarketItemsDetails, CCError**>;
const auto jsb_CCSoomlaStore_restoreTransactions =
    &ee::core::jsb_method_call<CCSoomlaStore,
                               &CCSoomlaStore::restoreTransactions>;
SE_BIND_FUNC(jsb_CCSoomlaStore_initialize)
SE_BIND_FUNC(jsb_CCSoomlaStore_getInstance)
SE_BIND_FUNC(jsb_CCSoomlaStore_refreshMarketItemsDetails)
SE_BIND_FUNC(jsb_CCSoomlaStore_restoreTransactions)

bool register_cc_soomla_store_manual(se::Object* globalObj) {
    se::Object* soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &soomlaObj);

    auto cls = se::Class::create("CCSoomlaStore", soomlaObj, nullptr, nullptr);
    cls->defineFunction("refreshMarketItemsDetails",
                        _SE(jsb_CCSoomlaStore_refreshMarketItemsDetails));
    cls->defineFunction("restoreTransactions",
                        _SE(jsb_CCSoomlaStore_restoreTransactions));
    cls->install();

    JSBClassType::registerClass<soomla::CCSoomlaStore>(cls);

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (soomlaObj->getProperty("CCSoomlaStore", &ctorVal) &&
        ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction("initialize",
                                           _SE(jsb_CCSoomlaStore_initialize));
        ctorVal.toObject()->defineFunction("getInstance",
                                           _SE(jsb_CCSoomlaStore_getInstance));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
