//
//  jsb_cc_virtual_currency_pack_builder.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "ee/jsb/soomla/jsb_cc_virtual_currency_pack_builder.hpp"

#include "Soomla/domain/virtualCurrencies/CCVirtualCurrencyPackBuilder.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace soomla {
se::Object* __jsb_CCVirtualCurrencyPackBuilder_proto = nullptr;
se::Class* __jsb_CCVirtualCurrencyPackBuilder_class = nullptr;

const auto jsb_CCVirtualCurrencyPackBuilder_finalize =
    &ee::core::jsb_finalize<CCVirtualCurrencyPackBuilder>;
const auto jsb_CCVirtualCurrencyPackBuilder_constructor =
    &ee::core::jsb_constructor<CCVirtualCurrencyPackBuilder>;
const auto jsb_CCVirtualCurrencyPackBuilder_setName =
    &ee::core::jsb_accessor_set<CCVirtualCurrencyPackBuilder,
                                &CCVirtualCurrencyPackBuilder::setName,
                                const std::string&>;
const auto jsb_CCVirtualCurrencyPackBuilder_setDescription =
    &ee::core::jsb_accessor_set<CCVirtualCurrencyPackBuilder,
                                &CCVirtualCurrencyPackBuilder::setDescription,
                                const std::string&>;
const auto jsb_CCVirtualCurrencyPackBuilder_setItemId =
    &ee::core::jsb_accessor_set<CCVirtualCurrencyPackBuilder,
                                &CCVirtualCurrencyPackBuilder::setItemId,
                                const std::string&>;
const auto jsb_CCVirtualCurrencyPackBuilder_setCurrencyAmount =
    &ee::core::jsb_accessor_set<
        CCVirtualCurrencyPackBuilder,
        &CCVirtualCurrencyPackBuilder::setCurrencyAmount, int>;
const auto jsb_CCVirtualCurrencyPackBuilder_setCurrencyItemId =
    &ee::core::jsb_accessor_set<
        CCVirtualCurrencyPackBuilder,
        &CCVirtualCurrencyPackBuilder::setCurrencyItemId, const std::string&>;
const auto jsb_CCVirtualCurrencyPackBuilder_setPurchaseType =
    &ee::core::jsb_accessor_set<CCVirtualCurrencyPackBuilder,
                                &CCVirtualCurrencyPackBuilder::setPurchaseType,
                                CCPurchaseType*>;
const auto jsb_CCVirtualCurrencyPackBuilder_build =
    &ee::core::jsb_method_get<CCVirtualCurrencyPackBuilder,
                              &CCVirtualCurrencyPackBuilder::build,
                              CCVirtualCurrencyPack*>;

SE_BIND_FINALIZE_FUNC(jsb_CCVirtualCurrencyPackBuilder_finalize)
SE_BIND_CTOR(jsb_CCVirtualCurrencyPackBuilder_constructor,
             __jsb_CCVirtualCurrencyPackBuilder_class,
             jsb_CCVirtualCurrencyPackBuilder_finalize)
SE_BIND_FUNC(jsb_CCVirtualCurrencyPackBuilder_setName)
SE_BIND_FUNC(jsb_CCVirtualCurrencyPackBuilder_setDescription)
SE_BIND_FUNC(jsb_CCVirtualCurrencyPackBuilder_setItemId)
SE_BIND_FUNC(jsb_CCVirtualCurrencyPackBuilder_setCurrencyAmount)
SE_BIND_FUNC(jsb_CCVirtualCurrencyPackBuilder_setCurrencyItemId)
SE_BIND_FUNC(jsb_CCVirtualCurrencyPackBuilder_setPurchaseType)
SE_BIND_FUNC(jsb_CCVirtualCurrencyPackBuilder_build)

bool register_cc_virtual_currency_pack_builder_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls =
        se::Class::create("CCVirtualCurrencyPackBuilder", __soomlaObj, nullptr,
                          _SE(jsb_CCVirtualCurrencyPackBuilder_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCVirtualCurrencyPackBuilder_finalize));

    cls->defineFunction("setName",
                        _SE(jsb_CCVirtualCurrencyPackBuilder_setName));
    cls->defineFunction("setDescription",
                        _SE(jsb_CCVirtualCurrencyPackBuilder_setDescription));
    cls->defineFunction("setItemId",
                        _SE(jsb_CCVirtualCurrencyPackBuilder_setItemId));
    cls->defineFunction(
        "setCurrencyAmount",
        _SE(jsb_CCVirtualCurrencyPackBuilder_setCurrencyAmount));
    cls->defineFunction(
        "setCurrencyItemId",
        _SE(jsb_CCVirtualCurrencyPackBuilder_setCurrencyItemId));
    cls->defineFunction("setPurchaseType",
                        _SE(jsb_CCVirtualCurrencyPackBuilder_setPurchaseType));
    cls->defineFunction("build", _SE(jsb_CCVirtualCurrencyPackBuilder_build));

    cls->install();

    JSBClassType::registerClass<CCVirtualCurrencyPackBuilder>(cls);

    __jsb_CCVirtualCurrencyPackBuilder_proto = cls->getProto();
    __jsb_CCVirtualCurrencyPackBuilder_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
