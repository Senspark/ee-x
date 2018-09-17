//
//  jsb_cc_virtual_item.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "CCVirtualItem.h"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_cc_virtual_item.hpp"
#include "jsb_core_common.hpp"

namespace ee {
namespace core {} // namespace core
} // namespace ee

namespace soomla {

static se::Object* __jsb_CCVirtualItem_proto = nullptr;
static se::Class* __jsb_CCVirtualItem_class = nullptr;
static se::Object* __soomlaObj = nullptr;

const auto jsb_CCVirtualItem_finalize = &ee::core::jsb_finalize<CCVirtualItem>;
const auto jsb_CCVirtualItem_getItemId =
    &ee::core::jsb_method_get<CCVirtualItem, &CCVirtualItem::getItemId,
                              std::string>;
const auto jsb_CCVirtualItem_give =
    &ee::core::jsb_method_get<CCVirtualItem,
                              (int (CCVirtualItem::*)(int, CCError**)) &
                                  CCVirtualItem::give,
                              int, int, CCError**>;
const auto jsb_CCVirtualItem_take =
    &ee::core::jsb_method_get<CCVirtualItem,
                              (int (CCVirtualItem::*)(int, CCError**)) &
                                  CCVirtualItem::take,
                              int, int, CCError**>;
const auto jsb_CCVirtualItem_resetBalance =
    &ee::core::jsb_method_get<CCVirtualItem,
                              (int (CCVirtualItem::*)(int, CCError**)) &
                                  CCVirtualItem::resetBalance,
                              int, int, CCError**>;
const auto jsb_CCVirtualItem_getBalance =
    &ee::core::jsb_method_get<CCVirtualItem,
                              (int (CCVirtualItem::*)(CCError**)) &
                                  CCVirtualItem::getBalance,
                              int, CCError**>;
const auto jsb_CCVirtualItem_save =
    &ee::core::jsb_method_call<CCVirtualItem, &CCVirtualItem::save, bool>;

SE_BIND_FINALIZE_FUNC(jsb_CCVirtualItem_finalize)
SE_BIND_FUNC(jsb_CCVirtualItem_getItemId)
SE_BIND_FUNC(jsb_CCVirtualItem_give)
SE_BIND_FUNC(jsb_CCVirtualItem_take)
SE_BIND_FUNC(jsb_CCVirtualItem_resetBalance)
SE_BIND_FUNC(jsb_CCVirtualItem_getBalance)
SE_BIND_FUNC(jsb_CCVirtualItem_save)

bool register_cc_virtual_item_manual(se::Object* globalObj) {
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls =
        se::Class::create("CCVirtualItem", __soomlaObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCVirtualItem_finalize));

    cls->defineFunction("getItemId", _SE(jsb_CCVirtualItem_getItemId));
    cls->defineFunction("give", _SE(jsb_CCVirtualItem_give));
    cls->defineFunction("take", _SE(jsb_CCVirtualItem_take));
    cls->defineFunction("resetBalance", _SE(jsb_CCVirtualItem_resetBalance));
    cls->defineFunction("getBalance", _SE(jsb_CCVirtualItem_getBalance));
    cls->defineFunction("save", _SE(jsb_CCVirtualItem_save));

    cls->install();

    JSBClassType::registerClass<CCVirtualItem>(cls);

    __jsb_CCVirtualItem_proto = cls->getProto();
    __jsb_CCVirtualItem_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
