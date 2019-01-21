//
//  jsb_cc_lifetime_vg_builder.hpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "ee/jsb/soomla/jsb_cc_lifetime_vg_builder.hpp"

#include "Soomla/domain/virtualGoods/CCLifetimeVGBuilder.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/soomla/jsb_cc_purchase_with_market_builder.hpp"

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, soomla::CCLifetimeVGBuilder& input) {
    value.toObject()->setPrivateData(&input);
}
} // namespace core
} // namespace ee

namespace soomla {
se::Class* __jsb_CCLifetimeVGBuilder_class = nullptr;

const auto jsb_CCLifetimeVGBuilder_finalize =
    &ee::core::jsb_finalize<CCLifetimeVGBuilder>;
const auto jsb_CCLifetimeVGBuilder_constructor =
    &ee::core::jsb_constructor<CCLifetimeVGBuilder>;

const auto jsb_CCLifetimeVGBuilder_build =
    &ee::core::jsb_method_get<CCLifetimeVGBuilder, &CCLifetimeVGBuilder::build,
                              CCVirtualItem*>;

const auto jsb_CCLifetimeVGBuilder_setItemId =
    &ee::core::jsb_accessor_set<CCLifetimeVGBuilder,
                                &CCLifetimeVGBuilder::setItemId, std::string>;

const auto jsb_CCLifetimeVGBuilder_setDescription = &ee::core::jsb_accessor_set<
    CCLifetimeVGBuilder, &CCLifetimeVGBuilder::setDescription, std::string>;

const auto jsb_CCLifetimeVGBuilder_setName =
    &ee::core::jsb_accessor_set<CCLifetimeVGBuilder,
                                &CCLifetimeVGBuilder::setName, std::string>;

const auto jsb_CCLifetimeVGBuilder_setPurchaseType =
    &ee::core::jsb_accessor_set<CCLifetimeVGBuilder,
                                &CCLifetimeVGBuilder::setPurchaseType,
                                CCPurchaseType*>;

SE_BIND_FINALIZE_FUNC(jsb_CCLifetimeVGBuilder_finalize)
SE_BIND_CTOR(jsb_CCLifetimeVGBuilder_constructor,
             __jsb_CCLifetimeVGBuilder_class, jsb_CCLifetimeVGBuilder_finalize)
SE_BIND_FUNC(jsb_CCLifetimeVGBuilder_build)
SE_BIND_FUNC(jsb_CCLifetimeVGBuilder_setItemId)
SE_BIND_FUNC(jsb_CCLifetimeVGBuilder_setDescription)
SE_BIND_FUNC(jsb_CCLifetimeVGBuilder_setName)
SE_BIND_FUNC(jsb_CCLifetimeVGBuilder_setPurchaseType)

bool register_cc_lifetime_vg_builder_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCLifetimeVGBuilder", __soomlaObj, nullptr,
                                 _SE(jsb_CCLifetimeVGBuilder_constructor));
    cls->defineFinalizeFunction(_SE(jsb_CCLifetimeVGBuilder_finalize));

    cls->defineFunction("build", _SE(jsb_CCLifetimeVGBuilder_build));
    cls->defineFunction("setItemId", _SE(jsb_CCLifetimeVGBuilder_setItemId));
    cls->defineFunction("setDescription",
                        _SE(jsb_CCLifetimeVGBuilder_setDescription));
    cls->defineFunction("setName", _SE(jsb_CCLifetimeVGBuilder_setName));
    cls->defineFunction("setPurchaseType",
                        _SE(jsb_CCLifetimeVGBuilder_setPurchaseType));

    cls->install();

    JSBClassType::registerClass<CCLifetimeVGBuilder>(cls);

    __jsb_CCLifetimeVGBuilder_class = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
