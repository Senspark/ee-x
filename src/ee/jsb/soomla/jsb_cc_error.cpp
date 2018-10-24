//
//  jsb_cc_error.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/13/18.
//

#include "ee/jsb/soomla/jsb_cc_error.hpp"

#include "Soomla/CCError.h"

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace soomla {
static se::Object* __jsb_CCError_proto = nullptr;
static se::Class* __jsb_CCError_class = nullptr;
} // namespace soomla

namespace ee {
namespace core {

template <>
soomla::CCError** get_value(const se::Value& value) {
    if (value.isNull()) {
        return nullptr;
    } else
        return static_cast<soomla::CCError**>(
            value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, soomla::CCError* input) {
    set_value_from_pointer(value, input);
}
} // namespace core
} // namespace ee

namespace soomla {

const auto jsb_CCError_finalize = &ee::core::jsb_finalize<CCError>;
const static auto jsb_CCError_createWithValue =
    &ee::core::jsb_static_get<CCError*, &CCError::createWithValue,
                              cocos2d::Value>;
const auto jsb_CCError_getInfo =
    &ee::core::jsb_method_get<CCError, &CCError::getInfo, const char*>;

SE_BIND_FINALIZE_FUNC(jsb_CCError_finalize)
SE_BIND_FUNC(jsb_CCError_createWithValue)
SE_BIND_FUNC(jsb_CCError_getInfo)

bool register_cc_error_manual(se::Object* globalObj) {
    se::Object* __soomlaObj = nullptr;
    ee::core::getOrCreatePlainObject_r("soomla", globalObj, &__soomlaObj);

    auto cls = se::Class::create("CCError", __soomlaObj, nullptr, nullptr);
    cls->defineFinalizeFunction(_SE(jsb_CCError_finalize));

    cls->defineFunction("getInfo", _SE(jsb_CCError_getInfo));

    cls->install();

    JSBClassType::registerClass<CCError>(cls);

    __jsb_CCError_proto = cls->getProto();
    __jsb_CCError_class = cls;

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (__soomlaObj->getProperty("CCError", &ctorVal) && ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction("createWithValue",
                                           _SE(jsb_CCError_createWithValue));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}
} // namespace soomla