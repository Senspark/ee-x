//
//  jsb_cc_single_use_pack_vg.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/20/18.
//

#include "ee/jsb/soomla/jsb_cc_single_use_pack_vg.hpp"

#include "Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"
#include "Soomla/domain/virtualGoods/CCSingleUsePackVG.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/soomla/jsb_cc_purchasable_virtual_item.hpp"

namespace soomla {

se::Object* __jsb_CCSingleUsePackVG_proto = nullptr;
se::Class* __jsb_CCSingleUsePackVG_class = nullptr;

const auto jsb_CCSingleUsePackVG_finalize =
    &ee::core::jsb_finalize<CCSingleUsePackVG>;
const auto jsb_CCSingleUsePackVG_getGoodAmount =
    &ee::core::jsb_method_get<CCSingleUsePackVG,
                              &CCSingleUsePackVG::getGoodAmount, std::int32_t>;

SE_BIND_FINALIZE_FUNC(jsb_CCSingleUsePackVG_finalize)
SE_BIND_FUNC(jsb_CCSingleUsePackVG_getGoodAmount)

bool register_cc_single_use_pack_vg_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto purchasableVirtualItemProto = getCCPurchasableVirtualItemProto();
    auto cls = se::Class::create("CCSingleUsePackVG", __soomlaObj,
                                 purchasableVirtualItemProto, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCSingleUsePackVG_finalize));

    cls->defineFunction("getGoodAmount",
                        _SE(jsb_CCSingleUsePackVG_getGoodAmount));

    cls->install();

    JSBClassType::registerClass<CCSingleUsePackVG>(cls);

    __jsb_CCSingleUsePackVG_proto = cls->getProto();
    __jsb_CCSingleUsePackVG_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
