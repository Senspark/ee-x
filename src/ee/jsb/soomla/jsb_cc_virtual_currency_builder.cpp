//
//  jsb_cc_virtual_currency_pack_builder.cpp
//  ee-x
//
//  Created by Kiet Le on 9/19/18.
//

#include "CCVirtualCurrencyBuilder.h"
#include "ee/jsb/core/jsb_templates.hpp"
#include "jsb_cc_virtual_currency_pack_builder.hpp"
#include "jsb_core_common.hpp"

namespace soomla {
static se::Object* __jsb_CCVirtualCurrencyBuilder_proto = nullptr;
static se::Class* __jsb_CCVirtualCurrencyBuilder_class = nullptr;
static se::Object* __soomlaObj = nullptr;

const auto jsb_CCVirtualCurrencyBuilder_finalize =
    &ee::core::jsb_finalize<CCVirtualCurrencyBuilder>;
const auto jsb_CCVirtualCurrencyBuilder_constructor =
    &ee::core::jsb_constructor<CCVirtualCurrencyBuilder>;
const auto jsb_CCVirtualCurrencyBuilder_setName =
    &ee::core::jsb_accessor_set<CCVirtualCurrencyBuilder,
                                &CCVirtualCurrencyBuilder::setName,
                                const std::string&>;
const auto jsb_CCVirtualCurrencyBuilder_setDescription =
    &ee::core::jsb_accessor_set<CCVirtualCurrencyBuilder,
                                &CCVirtualCurrencyBuilder::setDescription,
                                const std::string&>;
const auto jsb_CCVirtualCurrencyBuilder_setItemId =
    &ee::core::jsb_accessor_set<CCVirtualCurrencyBuilder,
                                &CCVirtualCurrencyBuilder::setItemId,
                                const std::string&>;
const auto jsb_CCVirtualCurrencyBuilder_build =
    &ee::core::jsb_method_get<CCVirtualCurrencyBuilder,
                              &CCVirtualCurrencyBuilder::build, CCVirtualItem*>;

SE_BIND_FINALIZE_FUNC(jsb_CCVirtualCurrencyBuilder_finalize)
SE_BIND_CTOR(jsb_CCVirtualCurrencyBuilder_constructor,
             __jsb_CCVirtualCurrencyBuilder_class,
             jsb_CCVirtualCurrencyBuilder_finalize)
SE_BIND_FUNC(jsb_CCVirtualCurrencyBuilder_setName)
SE_BIND_FUNC(jsb_CCVirtualCurrencyBuilder_setDescription)
SE_BIND_FUNC(jsb_CCVirtualCurrencyBuilder_setItemId)
SE_BIND_FUNC(jsb_CCVirtualCurrencyBuilder_build)

bool register_cc_virtual_currency_builder_manual(se::Object* globalObj) {
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls =
        se::Class::create("CCVirtualCurrencyBuilder", __soomlaObj, nullptr,
                          _SE(jsb_CCVirtualCurrencyBuilder_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCVirtualCurrencyBuilder_finalize));

    cls->defineFunction("setName", _SE(jsb_CCVirtualCurrencyBuilder_setName));
    cls->defineFunction("setDescription",
                        _SE(jsb_CCVirtualCurrencyBuilder_setDescription));
    cls->defineFunction("setItemId",
                        _SE(jsb_CCVirtualCurrencyBuilder_setItemId));
    cls->defineFunction("build", _SE(jsb_CCVirtualCurrencyBuilder_build));

    cls->install();

    JSBClassType::registerClass<CCVirtualCurrencyBuilder>(cls);

    __jsb_CCVirtualCurrencyBuilder_proto = cls->getProto();
    __jsb_CCVirtualCurrencyBuilder_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace soomla
