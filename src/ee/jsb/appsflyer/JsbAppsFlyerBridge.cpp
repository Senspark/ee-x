//
//  JsbAppsFlyerBridge.cpp
//  ee-x
//
//  Created by eps on 12/05/19.
//

#include "ee/jsb/appsflyer/JsbAppsFlyerBridge.hpp"

#include "ee/appsflyer/internal/AppsFlyerBridge.hpp"
#include "ee/jsb/appsflyer/JsbIAppsFlyerBridge.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace appsflyer {
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
    auto appsflyer_ = core::getPath(global, "ee/appsflyer");

    auto cls = se::Class::create("Bridge", appsflyer_,
                                 getIBridgeClass()->getProto(),
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));
    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace appsflyer
} // namespace ee
