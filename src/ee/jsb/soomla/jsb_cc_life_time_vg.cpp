//
//  jsb_cc_life_time_vg.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/20/18.
//

#include "ee/jsb/soomla/jsb_cc_life_time_vg.hpp"

#include "Soomla/domain/virtualGoods/CCLifetimeVG.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/soomla/jsb_cc_purchasable_virtual_item.hpp"
#include "ee/jsb/soomla/jsb_cc_virtual_item.hpp"

namespace soomla {

se::Object* __jsb_CCLifetimeVG_proto = nullptr;
se::Class* __jsb_CCLifetimeVG_class = nullptr;

const auto jsb_CCLifetimeVG_finalize = &ee::core::jsb_finalize<CCLifetimeVG>;
const auto jsb_CCLifttimeVG_getBalance =
    &ee::core::jsb_method_get<CCLifetimeVG, &CCLifetimeVG::getBalance, int,
                              CCError**>;

SE_BIND_FINALIZE_FUNC(jsb_CCLifetimeVG_finalize)
SE_BIND_FUNC(jsb_CCLifttimeVG_getBalance)

bool register_cc_life_time_vg_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);
    auto purchasableVirtualItemProto = getCCPurchasableVirtualItemProto();
    auto cls = se::Class::create("CCLifetimeVG", __soomlaObj,
                                 purchasableVirtualItemProto, nullptr);

    cls->defineFinalizeFunction(_SE(jsb_CCLifetimeVG_finalize));

    cls->defineFunction("getBalance", _SE(jsb_CCLifttimeVG_getBalance));

    cls->install();

    JSBClassType::registerClass<CCLifetimeVG>(cls);

    __jsb_CCLifetimeVG_proto = cls->getProto();
    __jsb_CCLifetimeVG_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
