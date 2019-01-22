//
//  jsb_facebook_bridge_impl.cpp
//  ee-x
//
//  Created by senspark-dev on 12/13/18.
//

#include "ee/jsb/facebook/jsb_facebook_bridge_impl.hpp"

#include "ee/facebook/internal/FacebookBridge.hpp"
#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/facebook/jsb_ifacebook_bridge.hpp"

namespace ee {
namespace facebook {
namespace {
se::Class* clazz = nullptr;

using Self = Bridge;

// clang-format off
constexpr auto constructor = &core::makeConstructor<Self>;
constexpr auto finalize    = &core::makeFinalize<Self>;
// clang-format on

SE_BIND_CTOR(constructor, clazz, finalize)
SE_BIND_FINALIZE_FUNC(finalize)
} // namespace

bool register_facebook_bridge_impl_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;

    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("Bridge", facebookObj,
                                 getIFacebookBridgeClass()->getProto(),
                                 _SE(constructor));
    cls->defineFinalizeFunction(_SE(finalize));
    cls->install();

    JSBClassType::registerClass<Bridge>(cls);
    clazz = cls;

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace facebook
} // namespace ee
