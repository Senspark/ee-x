//
//  jsb_logger.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/12/18.
//

#include "ee/jsb/core/jsb_logger.hpp"

#include <unordered_map>

#include <ee/Core.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {

static se::Object* __jsb_Logger_proto = nullptr;
static se::Class* __jsb_Logger_class = nullptr;

static se::Object* __jsb_s_Logger = nullptr;

template <>
const Logger& get_value(const se::Value& value) {
    return *static_cast<Logger*>(value.toObject()->getPrivateData());
}

template <>
void set_value(se::Value& value, ee::Logger& input) {
    se::Object* obj = nullptr;
    if (&input == &input.getSystemLogger()) {
        if (__jsb_s_Logger == nullptr) {
            obj = se::Object::createObjectWithClass(__jsb_Logger_class);
            obj->setPrivateData(&input);
            obj->root();
            __jsb_s_Logger = obj;
        } else {
            obj = __jsb_s_Logger;
        }
    } else {
        obj = se::Object::createObjectWithClass(__jsb_Logger_class);
        obj->setPrivateData(new ee::Logger(input));
    }
    value.setObject(obj);
}

constexpr static auto jsb_Logger_finalize = &jsb_finalize<ee::Logger>;
constexpr static auto jsb_Logger_constructor =
    &jsb_constructor<ee::Logger, std::string>;
constexpr static auto jsb_Logger_getSystemLogger =
    &jsb_static_get<ee::Logger&, &Logger::getSystemLogger>;
constexpr static auto jsb_Logger_setSystemLogger =
    &jsb_static_call<&Logger::setSystemLogger, const ee::Logger&>;
constexpr static auto jsb_Logger_setEnabled =
    &jsb_accessor_set<ee::Logger, &ee::Logger::setEnabled, bool>;
constexpr static auto jsb_Logger_log =
    &jsb_method_call<ee::Logger,
                     (void (ee::Logger::*)(const ee::LogLevel&, std::string,
                                           ...) const) &
                         ee::Logger::log,
                     const ee::LogLevel&, std::string>;
constexpr static auto jsb_Logger_verbose =
    &jsb_method_call<ee::Logger, &ee::Logger::verbose, std::string>;
constexpr static auto jsb_Logger_debug =
    &jsb_method_call<ee::Logger, &ee::Logger::debug, std::string>;
constexpr static auto jsb_Logger_info =
    &jsb_method_call<ee::Logger, &ee::Logger::info, std::string>;
constexpr static auto jsb_Logger_warn =
    &jsb_method_call<ee::Logger, &ee::Logger::warn, std::string>;
constexpr static auto jsb_Logger_error =
    &jsb_method_call<ee::Logger, &ee::Logger::error, std::string>;
constexpr static auto jsb_Logger_assert =
    &jsb_method_call<ee::Logger, &ee::Logger::error, std::string>;

SE_BIND_FINALIZE_FUNC(jsb_Logger_finalize)
SE_BIND_CTOR(jsb_Logger_constructor, __jsb_Logger_class, jsb_Logger_finalize)
SE_BIND_FUNC(jsb_Logger_getSystemLogger)
SE_BIND_FUNC(jsb_Logger_setSystemLogger)
SE_BIND_FUNC(jsb_Logger_setEnabled)
SE_BIND_FUNC(jsb_Logger_log);
SE_BIND_FUNC(jsb_Logger_verbose)
SE_BIND_FUNC(jsb_Logger_debug)
SE_BIND_FUNC(jsb_Logger_info)
SE_BIND_FUNC(jsb_Logger_warn)
SE_BIND_FUNC(jsb_Logger_error)
SE_BIND_FUNC(jsb_Logger_assert)

bool register_logger_manual(se::Object* globalObj) {
    getOrCreatePlainObject_r("ee", globalObj, &__eeObj);
    getOrCreatePlainObject_r("core", __eeObj, &__coreObj);

    auto cls = se::Class::create("Logger", __coreObj, nullptr,
                                 _SE(jsb_Logger_constructor));
    cls->defineFinalizeFunction(_SE(jsb_Logger_finalize));

    // Define member functions, member properties
    cls->defineFunction("setEnabled", _SE(jsb_Logger_setEnabled));
    cls->defineFunction("log", _SE(jsb_Logger_log));
    cls->defineFunction("verbose", _SE(jsb_Logger_verbose));
    cls->defineFunction("debug", _SE(jsb_Logger_debug));
    cls->defineFunction("info", _SE(jsb_Logger_info));
    cls->defineFunction("warn", _SE(jsb_Logger_warn));
    cls->defineFunction("error", _SE(jsb_Logger_error));
    cls->defineFunction("assert", _SE(jsb_Logger_assert));

    // Install the class to JS virtual machine
    cls->install();

    JSBClassType::registerClass<ee::Logger>(cls);

    __jsb_Logger_proto = cls->getProto();
    __jsb_Logger_class = cls;

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (__coreObj->getProperty("Logger", &ctorVal) && ctorVal.isObject()) {
        ctorVal.toObject()->defineFunction("getSystemLogger",
                                           _SE(jsb_Logger_getSystemLogger));
        ctorVal.toObject()->defineFunction("setSystemLogger",
                                           _SE(jsb_Logger_setSystemLogger));
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace core
} // namespace ee
