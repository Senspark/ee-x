//
//  jsb_cc_soomla.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/13/18.
//

#include "ee/jsb/soomla/jsb_cc_soomla.hpp"

#include <Soomla/Cocos2dxCore.h>

#include <ee/jsb/JsbCore.hpp>

namespace soomla {

const auto jsb_CCSoomla_initialize =
    &ee::core::jsb_static_call<&CCSoomla::initialize, const std::string&>;

SE_BIND_FUNC(jsb_CCSoomla_initialize)

bool register_cc_soomla_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

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
