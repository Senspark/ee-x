//
//  jsb_cc_store_inventory.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/13/18.
//

#include "ee/soomla/internal/jsb_cc_store_inventory.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>
#include <soomla/CCStoreInventory.h>

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, soomla::CCStoreInventory* input) {
    set_value_from_pointer(value, input);
}
} // namespace core
} // namespace ee

namespace soomla {
se::Class* __jsb_CCStoreInventory_class = nullptr;

const auto jsb_CCStoreInventory_finalize =
    &ee::core::jsb_finalize<CCStoreInventory>;
const auto jsb_CCStoreInventory_constructor =
    &ee::core::jsb_constructor<CCStoreInventory>;
const auto jsb_CCStoreInventory_sharedStoreInventory =
    &ee::core::jsb_static_get<CCStoreInventory*,
                              &CCStoreInventory::sharedStoreInventory>;
const auto jsb_CCStoreInventory_buyItem =
    &ee::core::jsb_method_call<CCStoreInventory,
                               (void (CCStoreInventory::*)(const std::string&,
                                                           CCError**)) &
                                   CCStoreInventory::buyItem,
                               const std::string&, CCError**>;
const auto jsb_CCStoreInventory_getItemBalance =
    &ee::core::jsb_method_get<CCStoreInventory,
                              &CCStoreInventory::getItemBalance, int,
                              const std::string&, CCError**>;
const auto jsb_CCStoreInventory_giveItem =
    &ee::core::jsb_method_call<CCStoreInventory, &CCStoreInventory::giveItem,
                               const std::string&, int, CCError**>;
const auto jsb_CCStoreInventory_takeItem =
    &ee::core::jsb_method_call<CCStoreInventory, &CCStoreInventory::takeItem,
                               const std::string&, int, CCError**>;

SE_BIND_FINALIZE_FUNC(jsb_CCStoreInventory_finalize)
SE_BIND_CTOR(jsb_CCStoreInventory_constructor, __jsb_CCStoreInventory_class,
             jsb_CCStoreInventory_finalize)
SE_BIND_FUNC(jsb_CCStoreInventory_sharedStoreInventory)
SE_BIND_FUNC(jsb_CCStoreInventory_buyItem)
SE_BIND_FUNC(jsb_CCStoreInventory_getItemBalance)
SE_BIND_FUNC(jsb_CCStoreInventory_giveItem)
SE_BIND_FUNC(jsb_CCStoreInventory_takeItem)

bool register_cc_store_inventory_manual(se::Object* globalObj) {
    auto __soomlaObj = ee::JsbUtils::getPath(globalObj, "soomla");

    auto cls = se::Class::create("CCStoreInventory", __soomlaObj, nullptr,
                                 _SE(jsb_CCStoreInventory_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCStoreInventory_finalize));

    cls->defineFunction("buyItem", _SE(jsb_CCStoreInventory_buyItem));
    cls->defineFunction("getItemBalance",
                        _SE(jsb_CCStoreInventory_getItemBalance));
    cls->defineFunction("giveItem", _SE(jsb_CCStoreInventory_giveItem));
    cls->defineFunction("takeItem", _SE(jsb_CCStoreInventory_takeItem));

    cls->install();

    JSBClassType::registerClass<CCStoreInventory>(cls);

    __jsb_CCStoreInventory_class = cls;

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (__soomlaObj->getProperty("CCStoreInventory", &ctorVal) &&
        ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction(
            "sharedStoreInventory",
            _SE(jsb_CCStoreInventory_sharedStoreInventory));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla

#endif // EE_X_COCOS_JS