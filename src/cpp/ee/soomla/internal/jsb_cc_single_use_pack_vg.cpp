//
//  jsb_cc_single_use_pack_vg.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/20/18.
//

#include "ee/soomla/internal/jsb_cc_single_use_pack_vg.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>
#include <soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h>
#include <soomla/domain/virtualGoods/CCSingleUsePackVG.h>

#include "ee/soomla/internal/jsb_cc_purchasable_virtual_item.hpp"

namespace soomla {

const auto jsb_CCSingleUsePackVG_finalize =
    &ee::core::jsb_finalize<CCSingleUsePackVG>;
const auto jsb_CCSingleUsePackVG_getGoodAmount =
    &ee::core::jsb_method_get<CCSingleUsePackVG,
                              &CCSingleUsePackVG::getGoodAmount, std::int32_t>;

SE_BIND_FINALIZE_FUNC(jsb_CCSingleUsePackVG_finalize)
SE_BIND_FUNC(jsb_CCSingleUsePackVG_getGoodAmount)

bool register_cc_single_use_pack_vg_manual(se::Object* globalObj) {
    auto __soomlaObj = ee::JsbUtils::getPath(globalObj, "soomla");

    auto purchasableVirtualItemProto = getCCPurchasableVirtualItemProto();
    auto cls = se::Class::create("CCSingleUsePackVG", __soomlaObj,
                                 purchasableVirtualItemProto, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCSingleUsePackVG_finalize));

    cls->defineFunction("getGoodAmount",
                        _SE(jsb_CCSingleUsePackVG_getGoodAmount));

    cls->install();

    JSBClassType::registerClass<CCSingleUsePackVG>(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla

#endif // EE_X_COCOS_JS