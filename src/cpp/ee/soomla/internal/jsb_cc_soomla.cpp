//
//  jsb_cc_soomla.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/13/18.
//

#include "ee/soomla/internal/jsb_cc_soomla.hpp"

#ifdef EE_X_COCOS_JS
#include <ee/core/JsbUtils.hpp>
#include <soomla/Cocos2dxCore.h>

namespace soomla {
const auto jsb_CCSoomla_initialize =
    &ee::core::jsb_static_call<&CCSoomla::initialize, const std::string&>;

SE_BIND_FUNC(jsb_CCSoomla_initialize)

bool register_cc_soomla_manual(se::Object* globalObj) {
    auto __soomlaObj = ee::JsbUtils::getPath(globalObj, "soomla");

    auto cls = se::Class::create("CCSoomla", __soomlaObj, nullptr, nullptr);
    cls->install();

    JSBClassType::registerClass<CCSoomla>(cls);

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (__soomlaObj->getProperty("CCSoomla", &ctorVal) && ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction("initialize",
                                           _SE(jsb_CCSoomla_initialize));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla

#endif // EE_X_COCOS_JS