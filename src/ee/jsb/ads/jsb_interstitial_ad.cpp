//
//  jsb_interstitial_ad.cpp
//  ee-x
//
//  Created by Duc Nguyen on 7/17/18.
//

#include "ee/jsb/ads/jsb_interstitial_ad.hpp"

#include <unordered_map>

#include "ee/ads/IInterstitialAd.hpp"
#include "ee/jsb/core/SharedPtrHandler.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<IInterstitialAd>> handler;
} // namespace

template <>
std::shared_ptr<IInterstitialAd> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value, std::shared_ptr<IInterstitialAd>& input) {
    handler->setValue(value, input);
}

template <>
bool makeFinalize<IInterstitialAd>(se::State& state) {
    return handler->finalize(state);
}
} // namespace core

namespace ads {
namespace {
using Self = IInterstitialAd;

// clang-format off
constexpr auto finalize             = &core::makeFinalize<Self>;
constexpr auto isLoaded             = &core::makeInstanceMethodOnUiThreadAndWait<&Self::isLoaded>;
constexpr auto load                 = &core::makeInstanceMethodOnUiThread<&Self::load>;
constexpr auto show                 = &core::makeInstanceMethodOnUiThreadAndWait<&Self::show>;
constexpr auto setResultCallback    = &core::makeInstanceMethod<&Self::setResultCallback>;
constexpr auto setOnClickedCallback = &core::makeInstanceMethod<&Self::setOnClickedCallback>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(isLoaded)
SE_BIND_FUNC(load)
SE_BIND_FUNC(show)
SE_BIND_FUNC(setResultCallback)
SE_BIND_FUNC(setOnClickedCallback)
} // namespace

se::Class* getIInterstitialClass() {
    CCASSERT(core::handler != nullptr, "__jsb_InterstitialAd_class is null");
    return core::handler->getClass();
}

bool register_interstitial_ad_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* adsObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("ads", eeObj, &adsObj);

    auto cls = se::Class::create("IInterstitialAd", adsObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, isLoaded);
    EE_JSB_DEFINE_FUNCTION(cls, load);
    EE_JSB_DEFINE_FUNCTION(cls, show);
    EE_JSB_DEFINE_FUNCTION(cls, setResultCallback);
    EE_JSB_DEFINE_FUNCTION(cls, setOnClickedCallback);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    core::handler = core::SharedPtrHandler<Self>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ads
} // namespace ee
