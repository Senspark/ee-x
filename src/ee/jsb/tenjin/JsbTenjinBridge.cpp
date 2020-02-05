//
//  Created by eps on 12/6/19.
//

#include "ee/jsb/tenjin/JsbTenjinBridge.hpp"

#include <ee/jsb/JsbCore.hpp>
#include <ee/tenjin/private/TenjinBridge.hpp>

#include "ee/jsb/tenjin/JsbITenjinBridge.hpp"

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
    auto scope = core::getPath(global, "ee");
    auto parent = getIBridgeClass()->getProto();
    auto cls = se::Class::create("Tenjin", scope, parent, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));
    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace tenjin
} // namespace ee
