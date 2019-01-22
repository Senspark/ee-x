//
// jsb_vungle_bridge.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 9/4/18
//
//

#include "ee/jsb/vungle/jsb_vungle_bridge.hpp"

#include "ee/Vungle.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace vungle {
namespace {
se::Class* clazz = nullptr;

using Self = Vungle;

// clang-format off
constexpr auto constructor         = &core::makeConstructor<Self>;
constexpr auto finalize            = &core::makeFinalize<Self>;
constexpr auto createRewardedVideo = &core::makeInstanceMethodOnUiThreadAndWait<&Self::createRewardedVideo>;
// clang-format on

SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_FUNC(createRewardedVideo)
} // namespace

bool register_vungle_bridge_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* vungleObj = nullptr;
    core::getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    core::getOrCreatePlainObject_r("vungle", eeObj, &vungleObj);

    auto cls =
        se::Class::create("Vungle", vungleObj, nullptr, _SE(constructor));
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
