//
// jsb_iron_source_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/24/18
//
//

#include "ee/jsb/ironsource/jsb_iron_source_bridge.hpp"

#include "ee/IronSource.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace ironsource {
se::Class* clazz = nullptr;

using Self = IronSource;

// clang-format off
constexpr auto constructor          = &core::makeConstructor<Self>;
constexpr auto finalize             = &core::makeFinalize<Self>;
constexpr auto initialize           = &core::makeInstanceMethodOnUiThreadAndWait<&Self::initialize>;
constexpr auto createRewardedVideo  = &core::makeInstanceMethodOnUiThread<&Self::createRewardedVideo>;
constexpr auto createInterstitialAd = &core::makeInstanceMethodOnUiThread<&Self::createInterstitialAd>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(initialize)
SE_BIND_FUNC(createRewardedVideo)
SE_BIND_FUNC(createInterstitialAd)

bool register_iron_source_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* ironsourceObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ironsource", eeObj, &ironsourceObj);

    auto cls = se::Class::create("IronSource", ironsourceObj, nullptr,
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, initialize);
    EE_JSB_DEFINE_FUNCTION(cls, createRewardedVideo);
    EE_JSB_DEFINE_FUNCTION(cls, createInterstitialAd);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ironsource
} // namespace ee
