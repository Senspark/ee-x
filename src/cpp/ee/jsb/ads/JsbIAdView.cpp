//
//  jsb_adview.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/15/18.
//

#include "ee/jsb/ads/JsbIAdView.hpp"

#include <ee/ads/IAdView.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace core {
namespace {
std::unique_ptr<SharedPtrHandler<IAdView>> handler;
} // namespace

template <>
std::shared_ptr<IAdView> get_value(const se::Value& value) {
    return handler->getValue(value);
}

template <>
void set_value(se::Value& value, std::shared_ptr<IAdView>& input) {
    handler->setValue(value, input);
}

template <>
bool makeFinalize<IAdView>(se::State& state) {
    return handler->finalize(state);
}
} // namespace core

namespace ads {
namespace {
using Self = IAdView;

// clang-format off
constexpr auto finalize        = &core::makeFinalize<Self>;
constexpr auto isLoaded        = &core::makeInstanceMethodOnUiThreadAndWait<&Self::isLoaded>;
constexpr auto load            = &core::makeInstanceMethodOnUiThread<&Self::load>;
constexpr auto getAnchor       = &core::makeInstanceMethodOnUiThreadAndWait<&Self::getAnchor>;
constexpr auto setAnchor       = &core::makeInstanceMethodOnUiThread<&Self::setAnchor>;
constexpr auto getPosition     = &core::makeInstanceMethodOnUiThreadAndWait<&Self::getPosition>;
constexpr auto setPosition     = &core::makeInstanceMethodOnUiThread<&Self::setPosition>;
constexpr auto getSize         = &core::makeInstanceMethodOnUiThreadAndWait<&Self::getSize>;
constexpr auto setSize         = &core::makeInstanceMethodOnUiThread<&Self::setSize>;
constexpr auto setVisible      = &core::makeInstanceMethodOnUiThread<&Self::setVisible>;
constexpr auto setLoadCallback = &core::makeInstanceMethod<&Self::setLoadCallback>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(isLoaded)
SE_BIND_FUNC(load)
SE_BIND_FUNC(getAnchor)
SE_BIND_FUNC(setAnchor)
SE_BIND_FUNC(getPosition)
SE_BIND_FUNC(setPosition)
SE_BIND_FUNC(getSize)
SE_BIND_FUNC(setSize)
SE_BIND_FUNC(setVisible)
SE_BIND_FUNC(setLoadCallback)
} // namespace

se::Class* getIAdViewClass() {
    CCASSERT(core::handler != nullptr, "__jsb_AdView_class is null");
    return core::handler->getClass();
}

bool registerJsbIAdView(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = se::Class::create("IAdView", scope, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, isLoaded);
    EE_JSB_DEFINE_FUNCTION(cls, load);
    EE_JSB_DEFINE_FUNCTION(cls, getAnchor);
    EE_JSB_DEFINE_FUNCTION(cls, setAnchor);
    EE_JSB_DEFINE_FUNCTION(cls, getPosition);
    EE_JSB_DEFINE_FUNCTION(cls, setPosition);
    EE_JSB_DEFINE_FUNCTION(cls, getSize);
    EE_JSB_DEFINE_FUNCTION(cls, setSize);
    EE_JSB_DEFINE_FUNCTION(cls, setVisible);
    EE_JSB_DEFINE_FUNCTION(cls, setLoadCallback);

    cls->install();
    JSBClassType::registerClass<Self>(cls);
    core::handler = core::SharedPtrHandler<Self>::create(cls);

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace ads
} // namespace ee
