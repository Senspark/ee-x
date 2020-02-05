//
//  JsbAppsFlyerBridge.cpp
//  ee-x
//
//  Created by eps on 12/05/19.
//

#include "ee/jsb/apps_flyer/JsbAppsFlyerBridge.hpp"

#include <ee/apps_flyer/internal/AppsFlyerBridge.hpp>
#include <ee/jsb/JsbCore.hpp>

#include "ee/jsb/apps_flyer/JsbIAppsFlyerBridge.hpp"

namespace ee {
namespace apps_flyer {
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
    auto cls = se::Class::create("AppsFlyer", scope, parent, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));
    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace apps_flyer
} // namespace ee
