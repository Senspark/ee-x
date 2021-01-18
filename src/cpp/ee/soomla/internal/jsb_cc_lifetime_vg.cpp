//
//  jsb_cc_life_time_vg.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/20/18.
//

#include "ee/soomla/internal/jsb_cc_lifetime_vg.hpp"

#include <ee/core/JsbUtils.hpp>
#include <soomla/domain/virtualGoods/CCLifetimeVG.h>

#include "ee/soomla/internal/jsb_cc_purchasable_virtual_item.hpp"

namespace soomla {

const auto jsb_CCLifetimeVG_finalize = &ee::core::jsb_finalize<CCLifetimeVG>;
const auto jsb_CCLifttimeVG_getBalance =
    &ee::core::jsb_method_get<CCLifetimeVG, &CCLifetimeVG::getBalance, int,
                              CCError**>;

SE_BIND_FINALIZE_FUNC(jsb_CCLifetimeVG_finalize)
SE_BIND_FUNC(jsb_CCLifttimeVG_getBalance)

bool register_cc_lifetime_vg_manual(se::Object* globalObj) {
    auto __soomlaObj = ee::JsbUtils::getPath(globalObj, "soomla");
    auto purchasableVirtualItemProto = getCCPurchasableVirtualItemProto();
    auto cls = se::Class::create("CCLifetimeVG", __soomlaObj,
                                 purchasableVirtualItemProto, nullptr);

    cls->defineFinalizeFunction(_SE(jsb_CCLifetimeVG_finalize));

    cls->defineFunction("getBalance", _SE(jsb_CCLifttimeVG_getBalance));

    cls->install();

    JSBClassType::registerClass<CCLifetimeVG>(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
