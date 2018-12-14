//
//  jsb_facebook_bridge_impl.cpp
//  ee-x
//
//  Created by senspark-dev on 12/13/18.
//

#include "ee/jsb/facebook/jsb_facebook_bridge_impl.hpp"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_logger.hpp"
#include "ee/jsb/core/jsb_templates.hpp"
#include "ee/jsb/facebook/jsb_ifacebook_bridge.hpp"

#include "ee/facebook/internal/FacebookBridge.hpp"

namespace ee {
namespace facebook {
se::Class* __jsb_FacebookBridge_class = nullptr;

constexpr auto jsb_FacebookBridge_finalize = &core::jsb_finalize<Bridge>;
constexpr auto jsb_FacebookBridge_constructor = &core::jsb_constructor<Bridge>;

SE_BIND_FINALIZE_FUNC(jsb_FacebookBridge_finalize)
SE_BIND_CTOR(jsb_FacebookBridge_constructor, __jsb_FacebookBridge_class,
             jsb_FacebookBridge_finalize)

bool register_facebook_bridge_impl_manual(se::Object* globalObject) {
    se::Object* eeObj = nullptr;
    se::Object* facebookObj = nullptr;

    core::getOrCreatePlainObject_r("ee", globalObject, &eeObj);
    core::getOrCreatePlainObject_r("facebook", eeObj, &facebookObj);

    auto cls = se::Class::create("Bridge", facebookObj,
                                 getIFacebookBridgeClass()->getProto(),
                                 _SE(jsb_FacebookBridge_constructor));
    cls->defineFinalizeFunction(_SE(jsb_FacebookBridge_finalize));

    cls->install();

    JSBClassType::registerClass<Bridge>(cls);

    __jsb_FacebookBridge_class = cls;

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
} // namespace facebook
} // namespace ee
