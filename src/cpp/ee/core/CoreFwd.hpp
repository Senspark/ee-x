//
//  CoreFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_CORE_FWD_HPP
#define EE_X_CORE_FWD_HPP

#ifdef __cplusplus

#include "ee/core/Macro.hpp"

#if defined(EE_X_COROUTINE)// && __cplusplus >= 202002L // Check for C++20 or later
#include <coroutine>
using suspend_always = std::suspend_always;
using coroutine_handle = std::coroutine_handle<>;
using coroutine_handle_void = std::coroutine_handle<void>;
using suspend_never = std::suspend_never;
#else
#include <experimental/coroutine>
using suspend_always = std::experimental::suspend_always;
using coroutine_handle = std::experimental::coroutine_handle<>;
using coroutine_handle_void = std::experimental::coroutine_handle<void>;
using suspend_never = std::experimental::suspend_never;
#endif // __cplusplus >= 202002L

namespace cppcoro {
class async_mutex;
class async_mutex_lock;
class async_mutex_lock_operation;
class async_mutex_scoped_lock_operation;
} // namespace cppcoro

namespace ee {
namespace core {
enum class Library;
class IPlugin;
class PluginManager;
class IPluginManagerImpl;

enum class LogLevel;
class ILogger;
class Logger;

class IMessageBridge;

class IService;
class ServiceLocator;

class ScopeGuard;
class SpinLock;

struct SafeInset;

class Platform;
class Thread;
class IThreadImpl;

class VideoPlayer;
class VideoPlayerManager;

template <class T>
class DynamicValue;

class ObserverHandle;

template <class Observer>
class ObserverBinder;

template <class Observer>
class IObserverManager;

template <class Observer>
class ObserverManager;

template <class Observer>
class SafeObserverManager;

template <class T = void>
class Task;

template <class T = void>
class LambdaAwaiter;

using AsyncMutex = cppcoro::async_mutex;
using AsyncMutexLockOperation = cppcoro::async_mutex_lock_operation;
using AsyncMutexScopedLockOperation =
    cppcoro::async_mutex_scoped_lock_operation;

class Delay;
class SwitchToLibraryThread;
class SwitchToMainThread;

#if defined(EE_X_ANDROID)
class JniMethodInfo;
class JniString;
class JniUtils;
#endif // EE_X_ANDROID

class JsbUtils;
} // namespace core

using core::IPlugin;
using core::Library;
using core::PluginManager;

using core::ILogger;
using core::Logger;
using core::LogLevel;

using core::IMessageBridge;

using core::IService;
using core::ServiceLocator;

using core::DynamicValue;
using core::ScopeGuard;
using core::SpinLock;

using core::Platform;
using core::SafeInset;
using core::Thread;

using core::VideoPlayer;
using core::VideoPlayerManager;

using core::ObserverBinder;
using core::ObserverHandle;

using core::IObserverManager;
using core::ObserverManager;
using core::SafeObserverManager;

using core::AsyncMutex;
using core::AsyncMutexLockOperation;
using core::AsyncMutexScopedLockOperation;
using core::Delay;
using core::LambdaAwaiter;
using core::SwitchToLibraryThread;
using core::SwitchToMainThread;
using core::Task;

using SwitchToUiThread [[deprecated]] = SwitchToMainThread;

#if defined(EE_X_ANDROID)
using core::JniMethodInfo;
using core::JniString;
using core::JniUtils;
#endif // EE_X_ANDROID

using core::JsbUtils;
} // namespace ee

/// JSB.
namespace se {
class Object;
class Value;
class Class;
} // namespace se

#endif // __cplusplus

#endif /* EE_X_CORE_FWD_HPP */
