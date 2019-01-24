//
//  jsb_templates.hpp
//  hello_world-mobile
//
//  Created by Duc Nguyen on 7/13/18.
//

#ifndef JSB_TEMPLATES_HPP__
#define JSB_TEMPLATES_HPP__

#include <type_traits>
#include <unordered_map>

#include "ee/jsb/core/jsb_utils.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/Core.hpp>

#include <cocos/scripting/js-bindings/jswrapper/SeApi.h>
#include <cocos/scripting/js-bindings/manual/jsb_conversions.hpp>
#include <cocos/scripting/js-bindings/manual/jsb_helper.hpp>

#define EE_JSB_DEFINE_FUNCTION(cls, name) cls->defineFunction(#name, _SE(name))

namespace ee {
namespace core {
/// Converts between se::Object and normal value.
template <typename T>
se::Object* create_JSON_object(const T& value);

/// Converts between se::Object and normal value.
template <typename T>
T from_JSON_object(se::Object* object);

/// Converts between se::Value and normal value.
template <typename T>
T get_value(const se::Value& value);

/// Converts between se::Value and normal value.
template <typename T>
void set_value(se::Value& value, T input);

template <typename T>
void set_value_from_pointer(se::Value& value, T* input) {
    if (input == nullptr) {
        value.setNull();
        return;
    }
    auto clazz = JSBClassType::findClass(input);
    CCASSERT(clazz, "ERROR: Class is not registered yet.");
    se::Object* obj = nullptr;
    auto found = se::NativePtrToObjectMap::find(input);
    if (found != se::NativePtrToObjectMap::end()) {
        obj = found->second;
    } else {
        obj = se::Object::createObjectWithClass(clazz);
        obj->setPrivateData(input);
    }
    value.setObject(obj);

    if constexpr (std::is_convertible<T*, cocos2d::Ref*>::value) {
        static_cast<cocos2d::Ref*>(input)->retain();
    }
}

namespace internal {
template <class T>
se::Value to_value(T&& arg) {
    se::Value value;
    set_value(value, std::forward<T>(arg));
    return value;
}

/// https://stackoverflow.com/questions/28410697/c-convert-vector-to-tuple
template <class T, std::size_t... Indices>
decltype(auto) makeTupleImpl(const std::vector<T>& args,
                             std::index_sequence<Indices...>) {
    return std::make_tuple(args[Indices]...);
}

template <std::size_t N, class T>
decltype(auto) makeTuple(const std::vector<T>& args) {
    return makeTupleImpl(args, std::make_index_sequence<N>());
}

/// https://stackoverflow.com/questions/36797770/get-function-parameters-count
template <class ResultType, class... ArgTypes>
struct FunctionTraits {
    using Result = ResultType;
    using Args = std::tuple<ArgTypes...>;
};

template <class ResultType, class InstanceType, class... ArgTypes>
struct MemberFunctionTraits : FunctionTraits<ResultType, ArgTypes...> {
    using Instance = InstanceType;
};

template <class PropertyType>
struct PropertyTraits {
    using Property = PropertyType;
};

template <class PropertyType, class InstanceType>
struct MemberPropertyTraits {
    using Property = PropertyType;
    using Instance = InstanceType;
};

/// Static functions.
template <class Result, class... Args>
constexpr auto makeFunctionTraits(Result(Args...)) {
    return FunctionTraits<Result, Args...>();
}

/// Member functions.
template <class Result, class Instance, class... Args>
constexpr auto makeFunctionTraits(Result (Instance::*)(Args...)) {
    return MemberFunctionTraits<Result, Instance, Args...>();
}

/// Const member functions.
template <class Result, class Instance, class... Args>
constexpr auto makeFunctionTraits(Result (Instance::*)(Args...) const) {
    return MemberFunctionTraits<Result, Instance, Args...>();
}

/// Static properties.
template <class Property>
constexpr auto makePropertyTraits(Property*) {
    return PropertyTraits<Property>();
}

/// Member properties.
template <class Property, class Instance>
constexpr auto makePropertyTraits(Property(Instance::*)) {
    return MemberPropertyTraits<Property, Instance>();
}

void callFunction(const se::Value& jsThis, const se::Value& jsFunc,
                  const se::ValueArray& args);
} // namespace internal

template <auto Function>
using FunctionInstance =
    typename decltype(internal::makeFunctionTraits(Function))::Instance;

template <auto Function>
using FunctionResult =
    typename decltype(internal::makeFunctionTraits(Function))::Result;

template <auto Function>
using FunctionArgs =
    typename decltype(internal::makeFunctionTraits(Function))::Args;

template <auto Function>
constexpr auto FunctionArity = std::tuple_size_v<FunctionArgs<Function>>;

template <auto Property>
using PropertyInstance =
    typename decltype(internal::makePropertyTraits(Property))::Instance;

template <auto Property>
using PropertyType =
    typename decltype(internal::makePropertyTraits(Property))::Property;

template <class T>
using UnifyType =
    std::conditional_t<std::is_pointer_v<T>, T, std::add_lvalue_reference_t<T>>;

namespace internal {
template <class T, class = void>
struct ArgumentParser {
    static decltype(auto) parse(const se::Value& jsThis, const se::Value& arg) {
        return get_value<std::decay_t<T>>(arg);
    }
};

template <class T>
struct ArgumentParser<T, std::void_t<decltype(&std::decay_t<T>::operator())>> {
    static decltype(auto) parse(const se::Value& jsThis, const se::Value& arg) {
        if (!arg.isObject() || !arg.toObject()->isFunction()) {
            SE_REPORT_ERROR("Wrong argument type.");
        }
        se::Value jsFunc(arg);
        jsFunc.toObject()->root();
        auto lambda = [=](auto... values) {
            // Must copy values.
            runOnCocosThread([=]() mutable {
                se::ScriptEngine::getInstance()->clearException();
                se::AutoHandleScope hs;
                se::ValueArray args = {internal::to_value(
                    std::forward<decltype(values)>(values))...};
                internal::callFunction(jsThis, jsFunc, args);
            });
        };
        return lambda;
    }
};

template <auto Function, std::size_t... Indices>
decltype(auto) callStaticFunctionImpl(const se::Value& jsThis,
                                      const se::ValueArray& args,
                                      std::index_sequence<Indices...>) {
    using Args = FunctionArgs<Function>;
    return std::invoke(
        Function, ArgumentParser<std::tuple_element_t<Indices, Args>>::parse(
                      jsThis, args[Indices])...);
}

template <auto Function, class Instance, std::size_t... Indices>
decltype(auto) callInstanceFunctionImpl(const se::Value& jsThis,
                                        Instance* instance,
                                        const se::ValueArray& args,
                                        std::index_sequence<Indices...>) {
    using Args = FunctionArgs<Function>;
    return std::invoke(
        Function, instance,
        ArgumentParser<std::tuple_element_t<Indices, Args>>::parse(
            jsThis, args[Indices])...);
}

template <class Instance, class... Args, std::size_t... Indices>
decltype(auto) makeObjectImpl(const se::ValueArray& args,
                              std::index_sequence<Indices...>) {
    return std::make_unique<Instance>(
        get_value<std::decay_t<Args>>(args[Indices])...);
}
} // namespace internal

/// Calls a static function.
template <auto Function>
decltype(auto) callStaticFunction(const se::Value& jsThis,
                                  const se::ValueArray& args) {
    constexpr auto Arity = FunctionArity<Function>;
    return internal::callStaticFunctionImpl<Function>(
        jsThis, args, std::make_index_sequence<Arity>());
}

/// Calls a member instance function.
template <auto Function, class Instance>
decltype(auto) callInstanceFunction(const se::Value& jsThis, Instance* instance,
                                    const se::ValueArray& args) {
    constexpr auto Arity = FunctionArity<Function>;
    return internal::callInstanceFunctionImpl<Function, Instance>(
        jsThis, instance, args, std::make_index_sequence<Arity>());
}

/// Instantiates a new object (unique_ptr).
template <class Instance, class... Args>
decltype(auto) makeObject(const se::ValueArray& args) {
    return internal::makeObjectImpl<Instance, Args...>(
        args, std::make_index_sequence<sizeof...(Args)>());
}

template <class Instance>
void makeDisposeCallback(Instance* instance) {
    se::Object* seObj = nullptr;

    auto iter = se::NativePtrToObjectMap::find(instance);
    if (iter != se::NativePtrToObjectMap::end()) {
        // Save se::Object pointer for being used in cleanup method.
        seObj = iter->second;
        // Unmap native and js object since native object was destroyed.
        // Otherwise, it may trigger 'assertion' in se::Object::setPrivateData
        // later since native obj is already released and the new native object
        // may be assigned with the same address.
        se::NativePtrToObjectMap::erase(iter);
    } else {
        return;
    }

    auto cleanup = [seObj]() {
        auto se = se::ScriptEngine::getInstance();
        if (!se->isValid() || se->isInCleanup())
            return;

        se::AutoHandleScope hs;
        se->clearException();

        // The mapping of native object & se::Object was cleared in above code.
        // The private data (native object) may be a different object associated
        // with other se::Object. Therefore, don't clear the mapping again.
        seObj->clearPrivateData(false);
        seObj->unroot();
        seObj->decRef();
    };

    if (!se::ScriptEngine::getInstance()->isGarbageCollecting()) {
        cleanup();
    } else {
        CleanupTask::pushTaskToAutoReleasePool(cleanup);
    }
}

template <class Instance, class... Args>
bool makeConstructor(se::State& state) {
    constexpr auto Arity = sizeof...(Args);
    auto&& args = state.args();
    if (Arity == args.size()) {
        auto object = makeObject<Instance, Args...>(args).release();
        state.thisObject()->setPrivateData(object);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <typename T, typename... Args>
[[deprecated("Use makeConstructor")]] bool jsb_constructor(se::State& s) {
    return makeConstructor<T, Args...>(s);
}

template <class Instance, class... Args>
bool makeConstructorWithDisposeCallback(se::State& state) {
    constexpr auto Arity = sizeof...(Args);
    auto&& args = state.args();
    if (Arity == args.size()) {
        auto object = makeObject<Instance, Args...>(args).release();
        object->setDisposeCallback(&makeDisposeCallback<Instance>);
        state.thisObject()->setPrivateData(object);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <typename T, typename... Args>
[[deprecated("Use makeConstructorWithDisposeCallback")]] bool
jsb_constructor_with_dispose_callback(se::State& s) {
    return makeConstructorWithDisposeCallback<T, Args...>(s);
}

template <class T>
bool makeFinalize(se::State& state) {
    auto&& object = state.nativeThisObject();
    if constexpr (std::is_convertible_v<T*, cocos2d::Ref*>) {
        static_cast<cocos2d::Ref*>(object)->release();
    } else {
        auto&& pointer = static_cast<std::shared_ptr<T>*>(object);
        if (pointer == nullptr) {
            delete static_cast<T*>(object);
        }
    }
    return true;
}

template <typename T>
[[deprecated("Use makeFinalize")]] bool jsb_finalize(se::State& s) {
    return makeFinalize<T>(s);
}

template <auto Function>
bool makeStaticMethod(se::State& state) {
    constexpr auto Arity = FunctionArity<Function>;
    auto&& args = state.args();
    if (Arity == args.size()) {
        se::Value jsThis(state.thisObject());
        using Result = FunctionResult<Function>;
        if constexpr (std::is_same_v<Result, void>) {
            callStaticFunction<Function>(jsThis, args);
        } else {
            auto&& result = callStaticFunction<Function>(jsThis, args);
            set_value<UnifyType<Result>>(state.rval(), result);
        }
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <auto Function>
bool makeInstanceMethod(se::State& state) {
    constexpr auto Arity = FunctionArity<Function>;
    auto&& args = state.args();
    if (Arity == args.size()) {
        se::Value jsThis(state.thisObject());
        using Instance = FunctionInstance<Function>;
        auto&& instance = static_cast<Instance*>(state.nativeThisObject());
        using Result = FunctionResult<Function>;
        if constexpr (std::is_same_v<Result, void>) {
            callInstanceFunction<Function>(jsThis, instance, args);
        } else {
            auto&& result =
                callInstanceFunction<Function>(jsThis, instance, args);
            set_value<UnifyType<Result>>(state.rval(), result);
        }
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <auto Function>
bool makeMethod(se::State& state) {
    if constexpr (std::is_member_function_pointer_v<decltype(Function)>) {
        return makeInstanceMethod<Function>(state);
    } else {
        return makeStaticMethod<Function>(state);
    }
}

template <auto Function, class = std::enable_if_t<
                             std::is_same_v<FunctionResult<Function>, void>>>
bool makeInstanceMethodOnUiThread(se::State& state) {
    constexpr auto Arity = FunctionArity<Function>;
    auto&& args = state.args();
    if (Arity == args.size()) {
        se::Value jsThis(state.thisObject());
        using Instance = FunctionInstance<Function>;
        auto&& instance = static_cast<Instance*>(state.nativeThisObject());
        using Result = FunctionResult<Function>;
        static_assert(std::is_same_v<Result, void>);
        runOnUiThread([jsThis, instance, args] {
            callInstanceFunction<Function>(jsThis, instance, args);
        });
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <auto Function>
bool makeInstanceMethodOnUiThreadAndWait(se::State& state) {
    constexpr auto Arity = FunctionArity<Function>;
    auto&& args = state.args();
    if (Arity == args.size()) {
        se::Value jsThis(state.thisObject());
        using Instance = FunctionInstance<Function>;
        auto&& instance = static_cast<Instance*>(state.nativeThisObject());
        using Result = FunctionResult<Function>;
        if constexpr (std::is_same_v<Result, void>) {
            runOnUiThreadAndWait([jsThis, instance, args] {
                callInstanceFunction<Function>(jsThis, instance, args);
            });
        } else {
            auto&& result =
                runOnUiThreadAndWaitResult<Result>([jsThis, instance, args] {
                    return callInstanceFunction<Function>(jsThis, instance,
                                                          args);
                });
            set_value<UnifyType<Result>>(state.rval(), result);
        }
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %zu.",
                    args.size(), Arity);
    return false;
}

template <auto Property>
bool makeStaticProperty(se::State& state) {
    using Type = PropertyType<Property>;
    set_value<UnifyType<Type>>(state.rval(), *Property);
    return true;
}

template <auto Property>
bool makeInstanceProperty(se::State& state) {
    using Instance = PropertyInstance<Property>;
    auto instance = static_cast<Instance*>(state.nativeThisObject());
    using Type = PropertyType<Property>;
    set_value<UnifyType<Type>>(state.rval(), instance->*Property);
    return true;
}

template <auto Function, typename... Args>
[[deprecated("Use makeStaticMethod")]] bool jsb_static_call(se::State& s) {
    return makeStaticMethod<Function>(s);
}

template <typename ReturnType, auto FunctionPtr, typename... Args>
[[deprecated("Use makeStaticMethod")]] bool jsb_static_get(se::State& s) {
    return makeStaticMethod<FunctionPtr>(s);
}

template <typename ReturnType, auto MemberPtr>
[[deprecated("Use makeStaticProperty")]] bool jsb_static_property_get(
    se::State& s) { //
    return makeStaticProperty<MemberPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
[[deprecated("Use makeInstanceMethod")]] bool jsb_method_call(se::State& s) {
    return makeInstanceMethod<FunctionPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename... Args>
[[deprecated("Use makeInstanceMethodOnUiThread")]] //
    bool jsb_method_call_on_ui_thread(se::State& s) {
        return makeInstanceMethodOnUiThread<FunctionPtr>(s);
    }

template <typename InstanceType, auto FunctionPtr, typename... Args>
[[deprecated("Use makeInstanceMethodOnUiThreadAndWait")]] //
    bool jsb_method_call_on_ui_thread_and_wait(se::State& s) {
        return makeInstanceMethodOnUiThreadAndWait<FunctionPtr>(s);
    }

template <typename InstanceType, auto FunctionPtr, typename ReturnType,
          typename... Args>
[[deprecated("Use makeInstanceMethod")]] bool jsb_method_get(se::State& s) {
    return makeInstanceMethod<FunctionPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType,
          typename... Args>
[[deprecated("Use makeInstanceMethodOnUiThreadAndWait")]] //
    bool jsb_method_get_on_ui_thread(se::State& s) {
        return makeInstanceMethodOnUiThreadAndWait<FunctionPtr>(s);
    }

template <typename InstanceType, auto FunctionPtr, typename ReturnType>
[[deprecated("Use makeInstanceMethod")]] bool jsb_accessor_get(se::State& s) {
    return makeInstanceMethod<FunctionPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename ReturnType>
[[deprecated("Use makeInstanceMethodOnUiThreadAndWait")]] //
    bool jsb_accessor_get_on_ui_thread(se::State& s) {
        return makeInstanceMethodOnUiThreadAndWait<FunctionPtr>(s);
    }

template <typename InstanceType, auto FunctionPtr, typename ArgumentType>
[[deprecated("Use makeInstanceMethod")]] bool jsb_accessor_set(se::State& s) {
    return makeInstanceMethod<FunctionPtr>(s);
}

template <typename InstanceType, auto FunctionPtr, typename ArgumentType>
[[deprecated("Use makeInstanceMethodOnUiThread")]] //
    bool jsb_accessor_set_on_ui_thread(se::State& s) {
        return makeInstanceMethodOnUiThread<FunctionPtr>(s);
    }

template <typename InstanceType, auto MemberPtr, typename ReturnType>
[[deprecated("Use makeInstanceProperty")]] bool jsb_propterty_get(
    se::State& s) { //
    return makeInstanceProperty<MemberPtr>(s);
}

template <typename InstanceType, auto MemberPtr, typename ArgumentType>
bool jsb_propterty_set(se::State& s) {
    const auto& args = s.args();
    if (args.size() == 1) {
        auto cObj = static_cast<InstanceType*>(s.nativeThisObject());
        cObj->*MemberPtr = get_value<std::decay_t<ArgumentType>>(args[0]);
        return true;
    }
    SE_REPORT_ERROR("Wrong number of arguments: %zu, was expecting: %d.",
                    args.size(), 1);
    return false;
}

template <class T>
class SharedPtrHandler {
public:
    static std::unique_ptr<SharedPtrHandler> create(se::Class* clazz) {
        return std::make_unique<SharedPtrHandler<T>>(clazz);
    }

    explicit SharedPtrHandler(se::Class* clazz)
        : clazz_(clazz) {}

    se::Class* getClass() {
        return clazz_;
    }

    std::shared_ptr<T> getValue(const se::Value& value) const {
        auto ptr = static_cast<T*>(value.toObject()->getPrivateData());
        auto&& iter = map_.find(ptr);
        if (iter == map_.cend()) {
            SE_REPORT_ERROR("Could not find shared_ptr.");
            return nullptr;
        }
        return iter->second;
    }

    void setValue(se::Value& value, const std::shared_ptr<T>& input) {
        if (input == nullptr) {
            value.setNull();
            return;
        }
        se::Object* obj = nullptr;
        auto ptr = input.get();
        auto iter = se::NativePtrToObjectMap::find(ptr);
        if (iter != se::NativePtrToObjectMap::end()) {
            obj = iter->second;
        } else {
            map_.emplace(ptr, input);
            obj = se::Object::createObjectWithClass(clazz_);
            obj->setPrivateData(ptr);
        }
        value.setObject(obj);
    }

    bool finalize(se::State& s) {
        auto ptr = static_cast<T*>(s.nativeThisObject());
        auto&& iter = map_.find(ptr);
        if (iter == map_.cend()) {
            SE_REPORT_ERROR("Could not find raw pointer.");
            delete ptr;
        } else {
            map_.erase(iter);
        }
        return true;
    }

private:
    se::Class* clazz_;

    /// Keeps strong reference to shared_ptr elements.
    std::unordered_map<T*, std::shared_ptr<T>> map_;
};
} // namespace core
} // namespace ee

#endif /* JSB_TEMPLATES_HPP__ */
