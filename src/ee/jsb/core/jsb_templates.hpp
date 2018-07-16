//
//  jsb_templates.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/13/18.
//

#ifndef JSB_TEMPLATES_HPP__
#define JSB_TEMPLATES_HPP__

#include <type_traits>

#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"

#include <ee/Core.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace core {

template <typename T> se::Object* create_JSON_object(const T& value);

template <>
inline se::Object* create_JSON_object(const std::pair<float, float>& value) {
    auto&& jsonArray = nlohmann::json::array();
    jsonArray.push_back(value.first);
    jsonArray.push_back(value.second);
    return se::Object::createJSONObject(jsonArray.dump());
}

template <>
inline se::Object* create_JSON_object(const std::pair<int, int>& value) {
    auto&& jsonArray = nlohmann::json::array();
    jsonArray.push_back(value.first);
    jsonArray.push_back(value.second);
    return se::Object::createJSONObject(jsonArray.dump());
}

template <typename T> T get_value(const se::Value& value);

template <> inline bool get_value(const se::Value& value) {
    return value.toBoolean();
}

template <> inline std::int32_t get_value(const se::Value& value) {
    return value.toInt32();
}

template <> inline float get_value(const se::Value& value) {
    return value.toFloat();
}

template <> inline std::string get_value(const se::Value& value) {
    return std::move(value.toString());
}

template <> inline const std::string& get_value(const se::Value& value) {
    return value.toString();
}

template <typename T> void set_value(se::Value& value, T input);

template <> inline void set_value(se::Value& value, std::int32_t input) {
    value.setInt32(input);
}

template <> inline void set_value(se::Value& value, std::string input) {
    value.setString(input);
}

template <> inline void set_value(se::Value& value, bool input) {
    value.setBoolean(input);
}

template <> inline void set_value(se::Value& value, se::Object* obj) {
    value.setObject(obj);
}

template <>
inline void set_value(se::Value& value, std::pair<float, float> input) {
    auto obj = create_JSON_object<std::pair<float, float>>(input);
    value.setObject(obj);
}

template <> inline void set_value(se::Value& value, std::pair<int, int> input) {
    auto obj = create_JSON_object<std::pair<int, int>>(input);
    value.setObject(obj);
}

template <auto FunctionPtr, typename... Args, std::size_t... Indices>
auto call_static_func(const se::ValueArray& args,
                      std::index_sequence<Indices...>) {
    return (*FunctionPtr)(get_value<Args>(args[Indices])...);
}

template <typename InstanceType, auto FunctionPtr, typename... Args,
          std::size_t... Indices>
auto call_instance_func(InstanceType* instance, const se::ValueArray& args,
                        std::index_sequence<Indices...>) {
    return (instance->*FunctionPtr)(get_value<Args>(args[Indices])...);
}

template <typename T, typename... Args, std::size_t... Indices>
auto make_object(const se::ValueArray& args, std::index_sequence<Indices...>) {
    return new T(get_value<Args>(args[Indices])...);
}

template <typename T, typename... Args>
static bool jsb_constructor(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();

    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = make_object<T, Args...>(args, Indices());
        s.thisObject()->setPrivateData(cObj);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.", argc,
                    2);
    return false;
}

template <typename T> static bool jsb_finalize(se::State& s) {
    T* cObj = static_cast<T*>(s.nativeThisObject());
    delete cObj;
    return true;
}

template <auto Function, typename... Args> bool jsb_static_call(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();

    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        call_static_func<Function, Args...>(args, Indices());
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename ReturnType, auto FunctionPtr, typename... Args>
bool jsb_static_get(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();

    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto result = call_static_func<FunctionPtr, Args...>(args, Indices());
        set_value<ReturnType>(s.rval(), result);
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename ReturnType, auto MemberPtr>
bool jsb_static_property_get(se::State& s) {
    set_value<ReturnType>(s.rval(), *MemberPtr);
    return true;
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
bool jsb_method_call(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        call_instance_func<InstanceType, FunctionPtr, Args...>(cObj, args,
                                                               Indices());
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
bool jsb_method_call_on_ui_thread(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        ee::runOnUiThread([cObj, args] {
            call_instance_func<InstanceType, FunctionPtr, Args...>(cObj, args,
                                                                   Indices());
        });
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
bool jsb_method_call_on_ui_thread_and_wait(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        ee::runOnUiThreadAndWait([cObj, args] {
            call_instance_func<InstanceType, FunctionPtr, Args...>(cObj, args,
                                                                   Indices());
        });
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType,
          typename... Args>
bool jsb_method_get(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        set_value<ReturnType>(
            s.rval(), call_instance_func<InstanceType, FunctionPtr, Args...>(
                          cObj, args, Indices()));
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType,
          typename... Args>
bool jsb_method_get_on_ui_thread(se::State& s) {
    auto argc = sizeof...(Args);
    const auto& args = s.args();
    using Indices = std::make_index_sequence<sizeof...(Args)>;

    if (argc == args.size()) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        set_value<ReturnType>(
            s.rval(),
            ee::runOnUiThreadAndWaitResult<ReturnType>([cObj,
                                                        args]() -> ReturnType {
                return std::forward<ReturnType>(call_instance_func<InstanceType, FunctionPtr, Args...>(
                    cObj, args, Indices()));
            }));
        return true;
    }

    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %ld.",
                    args.size(), argc);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType>
bool jsb_accessor_get(se::State& s) {
    auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
    set_value<ReturnType>(s.rval(), std::bind(FunctionPtr, cObj)());
    return true;
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType>
bool jsb_accessor_get_on_ui_thread(se::State& s) {
    auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
    set_value<ReturnType>(
        s.rval(),
        ee::runOnUiThreadAndWaitResult<ReturnType>(
            [cObj]() -> ReturnType { return std::bind(FunctionPtr, cObj)(); }));
    return true;
}

template <typename InstanceType, auto FunctionPtr, typename ArgumentType>
bool jsb_accessor_set(se::State& s) {
    const auto& args = s.args();
    if (args.size() == 1) {
        auto* cObj = static_cast<InstanceType*>(s.nativeThisObject());
        std::bind(FunctionPtr, cObj, get_value<ArgumentType>(args[0]))();
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.",
                    args.size(), 1);
    return false;
}

template <typename InstanceType, auto FunctionPtr, typename ArgumentType>
bool jsb_accessor_set_on_ui_thread(se::State& s) {
    const auto& args = s.args();
    if (args.size() == 1) {
        auto* cObj = static_cast<InstanceType*>(s.nativeThisObject());
        ee::runOnUiThread([cObj, args] {
            std::bind(FunctionPtr, cObj, get_value<ArgumentType>(args[0]))();
        });
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.",
                    args.size(), 1);
    return false;
}

template <typename InstanceType, auto MemberPtr, typename ReturnType>
bool jsb_propterty_get(se::State& s) {
    auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
    auto input(cObj->*MemberPtr);
    set_value<ReturnType>(s.rval(), std::move(input));
    return true;
}

template <typename InstanceType, auto MemberPtr, typename ArgumentType>
bool jsb_propterty_set(se::State& s) {
    const auto& args = s.args();
    if (args.size() == 1) {
        auto* cObj = static_cast<InstanceType*>(s.nativeThisObject());
        cObj->*MemberPtr = get_value<std::decay_t<ArgumentType>>(args[0]);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %ld, was expecting: %d.",
                    args.size(), 1);
    return false;
}

} // namespace core
} // namespace ee

#endif /* JSB_TEMPLATES_HPP__ */
