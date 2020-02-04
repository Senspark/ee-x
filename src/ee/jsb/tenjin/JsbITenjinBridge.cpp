//
//  Created by eps on 12/6/19.
//

#include "ee/jsb/tenjin/JsbITenjinBridge.hpp"

#include <ee/jsb/JsbCore.hpp>
#include <ee/tenjin/ITenjinBridge.hpp>

namespace ee {
namespace tenjin {
namespace {
se::Class* clazz = nullptr;

using Self = IBridge;

// clang-format off
constexpr auto finalize   = &core::makeFinalize<Self>;
constexpr auto initialize = &core::makeInstanceMethodOnUiThread<&Self::initialize>;
// clang-format on

SE_BIND_FINALIZE_FUNC(finalize);
SE_BIND_FUNC(initialize);
} // namespace

se::Class* getIBridgeClass() {
    return clazz;
}

bool registerJsbIBridge(se::Object* global) {
    auto scope = core::getPath(global, "ee");
    auto cls = se::Class::create("ITenjin", scope, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(finalize));

    EE_JSB_DEFINE_FUNCTION(cls, initialize);

    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace tenjin
} // namespace ee
