//
// jsb_vungle_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 9/4/18
//
//

#include "ee/jsb/vungle/JsbVungleBridge.hpp"

#include <ee/Vungle.hpp>
#include <ee/jsb/JsbCore.hpp>

namespace ee {
namespace vungle {
namespace {
se::Class* clazz = nullptr;

using Self = Bridge;

// clang-format off
constexpr auto constructor         = &core::makeConstructor<Self>;
constexpr auto finalize            = &core::makeFinalize<Self>;
constexpr auto createRewardedVideo = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createRewardedVideo>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FUNC(createRewardedVideo)
} // namespace

bool registerJsbBridge(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = se::Class::create("Vungle", scope, nullptr, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, createRewardedVideo);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace vungle
} // namespace ee
