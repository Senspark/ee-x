//
//  Created by eps on 12/6/19.
//

#include "ee/jsb/tenjin/JsbITenjinBridge.hpp"

#include "ee/tenjin/ITenjinBridge.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

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
    auto tenjin_ = core::getPath(global, "ee/tenjin");

    auto cls = se::Class::create("IBridge", tenjin_, nullptr, nullptr);
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
