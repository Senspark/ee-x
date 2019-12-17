//
//  Created by eps on 12/6/19.
//

#include "ee/jsb/tenjin/JsbTenjinBridge.hpp"

#include "ee/tenjin/internal/TenjinBridge.hpp"
#include "ee/jsb/tenjin/JsbITenjinBridge.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace tenjin {
namespace {
se::Class* clazz = nullptr;

using Self = Bridge;

// clang-format off
constexpr auto constructor = &core::makeConstructor<Self>;
constexpr auto finalize    = &core::makeFinalize<Self>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize)
SE_BIND_CTOR(constructor, clazz, finalize)
} // namespace

bool registerJsbBridge(se::Object* global) {
    auto tenjin_ = core::getPath(global, "ee/tenjin");

    auto cls = se::Class::create("Bridge", tenjin_,
                                 getIBridgeClass()->getProto(),
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));
    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace tenjin
} // namespace ee
