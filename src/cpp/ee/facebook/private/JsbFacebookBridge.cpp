//
//  jsb_facebook_bridge_impl.cpp
//  ee-x
//
//  Created by senspark-dev on 12/13/18.
//

#include "ee/facebook/private/JsbFacebookBridge.hpp"

#include <ee/core/JsbUtils.hpp>

#include "ee/facebook/private/FacebookBridge.hpp"
#include "ee/facebook/private/JsbIFacebookBridge.hpp"

namespace ee {
namespace facebook {
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
    auto scope = JsbUtils::getPath(global, "ee");
    auto parent = getIFacebookBridgeClass()->getProto();
    auto cls = se::Class::create("Facebook", scope, parent, _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));
    cls->install();

    JSBClassType::registerClass<Self>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
