//
//  jsb_cc_store_info.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "ee/jsb/soomla/jsb_cc_store_info.hpp"

#include "Soomla/Cocos2dxCore.h"
#include "Soomla/data/CCStoreInfo.h"
#include "Soomla/domain/CCPurchasableVirtualItem.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace soomla {
std::vector<se::Object*> __jsb__s_storeObjArchive;
} // namespace soomla

namespace ee {
namespace core {

template <>
soomla::CCStoreInfo* get_value(const se::Value& value) {
    return static_cast<soomla::CCStoreInfo*>(
        value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, soomla::CCStoreInfo* input) {
    set_value_from_pointer(value, input);
}

template <>
bool jsb_finalize<soomla::CCStoreInfo>(se::State& s) {
    auto* cObj = static_cast<soomla::CCStoreInfo*>(s.nativeThisObject());
    soomla::__jsb__s_storeObjArchive.clear();
    delete cObj;
    return true;
}
} // namespace core
} // namespace ee

namespace soomla {
se::Object* __jsb_CCStoreInfo_proto = nullptr;
se::Class* __jsb_CCStoreInfo_class = nullptr;

const auto jsb_CCStoreInfo_finalize = &ee::core::jsb_finalize<CCStoreInfo>;
const auto jsb_CCStoreInfo_constructor =
    &ee::core::jsb_constructor<CCStoreInfo>;
const auto jsb_CCStoreInfo_sharedStoreInfo =
    &ee::core::jsb_static_get<CCStoreInfo*, &CCStoreInfo::sharedStoreInfo>;
const auto jsb_CCStoreInfo_getItemByItemId =
    &ee::core::jsb_method_get<CCStoreInfo,
                              (CCVirtualItem *
                               (CCStoreInfo::*)(const std::string&)) &
                                  CCStoreInfo::getItemByItemId,
                              CCVirtualItem*, const std::string&>;
const auto jsb_CCStoreInfo_getPurchasableItemWithProductId =
    &ee::core::jsb_method_get<
        CCStoreInfo, &CCStoreInfo::getPurchasableItemWithProductId,
        CCPurchasableVirtualItem*, const std::string&, CCError**>;

SE_BIND_FINALIZE_FUNC(jsb_CCStoreInfo_finalize)
SE_BIND_CTOR(jsb_CCStoreInfo_constructor, __jsb_CCStoreInfo_class,
             jsb_CCStoreInfo_finalize)
SE_BIND_FUNC(jsb_CCStoreInfo_sharedStoreInfo)
SE_BIND_FUNC(jsb_CCStoreInfo_getItemByItemId)
SE_BIND_FUNC(jsb_CCStoreInfo_getPurchasableItemWithProductId)

bool register_cc_store_info_manual(se::Object* globalObj) {
    se::Object* soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &soomlaObj);

    auto cls = se::Class::create("CCStoreInfo", soomlaObj, nullptr, nullptr);

    cls->defineFunction("getItemByItemId",
                        _SE(jsb_CCStoreInfo_getItemByItemId));
    cls->defineFunction("getPurchasableItemWithProductId",
                        _SE(jsb_CCStoreInfo_getPurchasableItemWithProductId));

    cls->install();

    JSBClassType::registerClass<CCStoreInfo>(cls);

    __jsb_CCStoreInfo_proto = cls->getProto();
    __jsb_CCStoreInfo_class = cls;

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (soomlaObj->getProperty("CCStoreInfo", &ctorVal) && ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction(
            "sharedStoreInfo", _SE(jsb_CCStoreInfo_sharedStoreInfo));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
