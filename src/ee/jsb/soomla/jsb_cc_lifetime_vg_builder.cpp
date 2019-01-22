//
//  jsb_cc_lifetime_vg_builder.hpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "ee/jsb/soomla/jsb_cc_lifetime_vg_builder.hpp"

#include <Soomla/domain/virtualGoods/CCLifetimeVGBuilder.h>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/soomla/jsb_cc_purchase_with_market_builder.hpp"

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, soomla::CCLifetimeVGBuilder& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core
} // namespace ee

namespace soomla {
namespace {
se::Class* clazz = nullptr;

using Self = CCLifetimeVGBuilder;

// clang-format off
constexpr auto finalize        = &ee::core::makeFinalize<Self>;
constexpr auto constructor     = &ee::core::makeConstructor<Self>;
constexpr auto build           = &ee::core::makeInstanceMethod<&Self::build>;
constexpr auto setItemId       = &ee::core::makeInstanceMethod<&Self::setItemId>;
constexpr auto setDescription  = &ee::core::makeInstanceMethod<&Self::setDescription>;
constexpr auto setName         = &ee::core::makeInstanceMethod<&Self::setName>;
constexpr auto setPurchaseType = &ee::core::makeInstanceMethod<&Self::setPurchaseType>;
// clang-format on

SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(setItemId)
SE_BIND_FUNC(setDescription)
SE_BIND_FUNC(setName)
SE_BIND_FUNC(setPurchaseType)
SE_BIND_FUNC(build)
} // namespace

bool register_cc_lifetime_vg_builder_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCLifetimeVGBuilder", __soomlaObj, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, setItemId);
    EE_JSB_DEFINE_FUNCTION(cls, setDescription);
    EE_JSB_DEFINE_FUNCTION(cls, setName);
    EE_JSB_DEFINE_FUNCTION(cls, setPurchaseType);
    EE_JSB_DEFINE_FUNCTION(cls, build);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla
