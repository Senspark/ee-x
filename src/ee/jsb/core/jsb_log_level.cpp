//
//  jsb_log_level.cpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/13/18.
//

#include "ee/jsb/core/jsb_log_level.hpp"

#include <set>
#include <unordered_map>

#include <ee/Core.hpp>
#include <ee/CoreFwd.hpp>

#include "ee/jsb/core/jsb_core_common.hpp"
#include "ee/jsb/core/jsb_templates.hpp"

namespace ee {
namespace core {

se::Class* __jsb_LogLevel_class = nullptr;
se::Object* __jsb_LogLevel_proto = nullptr;
std::unordered_map<const LogLevel*, se::Object*> __jsb_s_loglevels;

template <>
const LogLevel& get_value(const se::Value& value) {
    auto retObj = static_cast<LogLevel*>(value.toObject()->getPrivateData());
    return *retObj;
}

template <>
void set_value(se::Value& value, const LogLevel& input) {
    se::Object* obj = nullptr;
    if (__jsb_s_loglevels.count(&input) != 0) {
        obj = __jsb_s_loglevels[&input];
    } else {
        obj = se::Object::createObjectWithClass(__jsb_LogLevel_class);
        obj->setPrivateData(new LogLevel(input));
    }
    value.setObject(obj);
}

bool jsb_LogLevel_finalize(se::State& s) {
    LogLevel* cObj = static_cast<LogLevel*>(s.nativeThisObject());
    CCLOG("DELETE LOG_LEVEL");

    if (cObj != &LogLevel::Verbose && cObj != &LogLevel::Info &&
        cObj != &LogLevel::Error && cObj != &LogLevel::Assert &&
        cObj != &LogLevel::Debug && cObj != &LogLevel::Warn) {
        delete cObj;
    }
    return true;
}

constexpr auto jsb_LogLevel_constructor =
    &jsb_constructor<LogLevel, std::int32_t, const std::string&>;
constexpr auto jsb_LogLevel_get_priority =
    &jsb_propterty_get<LogLevel, &LogLevel::priority, std::int32_t>;
constexpr auto jsb_LogLevel_set_priority =
    &jsb_propterty_set<LogLevel, &LogLevel::priority, const std::int32_t&>;
constexpr auto jsb_LogLevel_get_desc =
    &jsb_propterty_get<LogLevel, &LogLevel::desc, const std::string&>;
constexpr auto jsb_LogLevel_set_desc =
    &jsb_propterty_set<LogLevel, &LogLevel::desc, const std::string&>;
constexpr auto jsb_LogLevel_Verbose =
    &jsb_static_property_get<const LogLevel&, &LogLevel::Verbose>;
constexpr auto jsb_LogLevel_Debug =
    &jsb_static_property_get<const LogLevel&, &LogLevel::Debug>;
constexpr auto jsb_LogLevel_Info =
    &jsb_static_property_get<const LogLevel&, &LogLevel::Info>;
constexpr auto jsb_LogLevel_Warn =
    &jsb_static_property_get<const LogLevel&, &LogLevel::Warn>;
constexpr auto jsb_LogLevel_Error =
    &jsb_static_property_get<const LogLevel&, &LogLevel::Error>;
constexpr auto jsb_LogLevel_Assert =
    &jsb_static_property_get<const LogLevel&, &LogLevel::Assert>;

SE_BIND_FINALIZE_FUNC(jsb_LogLevel_finalize);
SE_BIND_CTOR(jsb_LogLevel_constructor, __jsb_LogLevel_class,
             jsb_LogLevel_finalize);
SE_BIND_PROP_GET(jsb_LogLevel_get_priority);
SE_BIND_PROP_SET(jsb_LogLevel_set_priority);
SE_BIND_PROP_GET(jsb_LogLevel_get_desc);
SE_BIND_PROP_SET(jsb_LogLevel_set_desc);
SE_BIND_PROP_GET(jsb_LogLevel_Verbose);
SE_BIND_PROP_GET(jsb_LogLevel_Debug);
SE_BIND_PROP_GET(jsb_LogLevel_Info);
SE_BIND_PROP_GET(jsb_LogLevel_Warn);
SE_BIND_PROP_GET(jsb_LogLevel_Error);
SE_BIND_PROP_GET(jsb_LogLevel_Assert);

bool register_log_level_manual(se::Object* globalObj) {
    se::Object* eeObj = nullptr;
    se::Object* coreObj = nullptr;
    getOrCreatePlainObject_r("ee", globalObj, &eeObj);
    getOrCreatePlainObject_r("core", eeObj, &coreObj);

    // Create a se::Class object, developers do not need to consider the release
    // of the se::Class object, which is automatically handled by the
    // ScriptEngine.
    auto cls = se::Class::create("LogLevel", coreObj, nullptr,
                                 _SE(jsb_LogLevel_constructor));

    // Define finalize callback function
    cls->defineFinalizeFunction(_SE(jsb_LogLevel_finalize));

    // Define member functions, member properties
    cls->defineProperty("priority", _SE(jsb_LogLevel_get_priority),
                        _SE(jsb_LogLevel_set_priority));
    cls->defineProperty("desc", _SE(jsb_LogLevel_get_desc),
                        _SE(jsb_LogLevel_set_desc));
    // Install the class to JS virtual machine
    cls->install();

    // JSBClassType::registerClass is a helper function in the Cocos2D-X native
    // binding code, which is not a part of the ScriptEngine.
    JSBClassType::registerClass<LogLevel>(cls);

    __jsb_LogLevel_proto = cls->getProto();
    __jsb_LogLevel_class = cls;

    // Register static member variables and static member functions
    se::Value ctorVal;
    if (coreObj->getProperty("LogLevel", &ctorVal) && ctorVal.isObject()) {
        ctorVal.toObject()->defineProperty("Verbose", _SE(jsb_LogLevel_Verbose),
                                           nullptr);
        ctorVal.toObject()->defineProperty("Debug", _SE(jsb_LogLevel_Debug),
                                           nullptr);
        ctorVal.toObject()->defineProperty("Info", _SE(jsb_LogLevel_Info),
                                           nullptr);
        ctorVal.toObject()->defineProperty("Warn", _SE(jsb_LogLevel_Warn),
                                           nullptr);
        ctorVal.toObject()->defineProperty("Error", _SE(jsb_LogLevel_Error),
                                           nullptr);
        ctorVal.toObject()->defineProperty("Assert", _SE(jsb_LogLevel_Assert),
                                           nullptr);
    }

    // Register predefined Loglevel instances
    const LogLevel* predefinedLogLevels[6] = {
        &core::LogLevel::Verbose, &LogLevel::Debug, &LogLevel::Info,
        &LogLevel::Warn,          &LogLevel::Error, &LogLevel::Assert,
    };

    for (auto logLevel : predefinedLogLevels) {
        __jsb_s_loglevels[logLevel] =
            se::Object::createObjectWithClass(__jsb_LogLevel_class);
        __jsb_s_loglevels[logLevel]->setPrivateData((void*)logLevel);
        __jsb_s_loglevels[logLevel]->root();
    }

    se::ScriptEngine::getInstance()->clearException();
    return true;
}

} // namespace core
} // namespace ee
