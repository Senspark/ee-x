//
//  jsb_cc_single_use_pack_vg.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/20/18.
//

#include "CCSingleUsePackVG.h"
#include "CCVirtualCurrencyPack.h"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_cc_purchasable_virtual_item.hpp"
#include "jsb_cc_single_use_pack_vg.hpp"
#include "jsb_core_common.hpp"

namespace ee {
namespace core {}
} // namespace ee

namespace soomla {

static se::Object* __jsb_CCSingleUsePackVG_proto = nullptr;
static se::Class* __jsb_CCSingleUsePackVG_class = nullptr;
static se::Object* __soomlaObj = nullptr;

const auto jsb_CCSingleUsePackVG_finalize =
    &ee::core::jsb_finalize<CCSingleUsePackVG>;
const auto jsb_CCSingleUsePackVG_getGoodAmount =
    &ee::core::jsb_method_get<CCSingleUsePackVG,
                              &CCSingleUsePackVG::getGoodAmount, std::int32_t>;

SE_BIND_FINALIZE_FUNC(jsb_CCSingleUsePackVG_finalize)
SE_BIND_FUNC(jsb_CCSingleUsePackVG_getGoodAmount)

bool register_cc_single_use_pack_vg_manual(se::Object* globalObj) {
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCSingleUsePackVG", __soomlaObj,
                                 __jsb_CCPurchasableVirtualItem_proto, nullptr);
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
