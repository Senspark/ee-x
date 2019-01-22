//
//  jsb_multi_adview.cpp
//  ee-x
//
//  Created by Kiet Le on 9/05/18.
//

#include "ee/jsb/ads/jsb_multi_adview.hpp"

#include "ee/ads/MultiAdView.hpp"
#include "ee/jsb/ads/jsb_adview.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
template <>
void set_value(se::Value& value, MultiAdView& input) {
    set_value_from_pointer(value, &input);
}
} // namespace core

namespace ads {
namespace {
se::Class* clazz = nullptr;

using Self = MultiAdView;

// clang-format off
constexpr auto finalize    = &core::makeFinalize<Self>;
constexpr auto constructor = &core::makeConstructor<Self>;
constexpr auto addItem     = &core::makeInstanceMethod<&Self::addItem>;
// clang-format on

SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(addItem)
} // namespace

bool register_multi_adview_manual(se::Object* globalObj) {
    se::Object* adsObj = nullptr;
    se::Object* eeObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ads", eeObj, &adsObj);

    auto cls = se::Class::create(
        "MultiAdView", adsObj, getIAdViewClass()->getProto(), _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, addItem);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ads
} // namespace ee
