//
//  jsb_cc_virtual_item.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "CCVirtualCurrencyPack.h"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_cc_purchasable_virtual_item.hpp"
#include "jsb_cc_virtual_curreny_pack.hpp"
#include "jsb_core_common.hpp"

namespace soomla {
se::Object* __jsb_CCVirtualCurrencyPack_proto = nullptr;
se::Class* __jsb_CCVirtualCurrencyPack_class = nullptr;
se::Object* __soomlaObj = nullptr;
} // namespace soomla

namespace ee {
namespace core {

template <>
void set_value(se::Value& value, soomla::CCVirtualCurrencyPack* input) {
    set_value_from_pointer(value, input);
}

} // namespace core
} // namespace ee

namespace soomla {

const auto jsb_CCVirtualCurrencyPack_finalize =
    &ee::core::jsb_finalize<CCVirtualCurrencyPack>;

SE_BIND_FINALIZE_FUNC(jsb_CCVirtualCurrencyPack_finalize)

bool register_cc_virtual_currency_manual(se::Object* globalObj) {
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCVirtualCurrencyPack", __soomlaObj,
                                 __jsb_CCPurchasableVirtualItem_proto, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCVirtualCurrencyPack_finalize));

    cls->install();

    JSBClassType::registerClass<CCVirtualCurrencyPack>(cls);

    __jsb_CCVirtualCurrencyPack_proto = cls->getProto();
    __jsb_CCVirtualCurrencyPack_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
