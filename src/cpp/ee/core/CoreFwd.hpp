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

namespace cppcoro {
class async_mutex;
class async_mutex_lock;
class async_mutex_lock_operation;
class async_mutex_scoped_lock_operation;
} // namespace cppcoro

namespace ee {
namespace core {
enum class Library;
enum class Plugin;
class IPlugin;
class PluginManager;

struct LogLevel;
class Logger;

class IMessageBridge;
class MessageBridge;

class IService;
class ServiceLocator;

class ScopeGuard;
class SpinLock;

struct SafeInset;

class Platform;
class Thread;

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

class SwitchToLibraryThread;
class SwitchToMainThread;

#if defined(EE_X_ANDROID)
class JniMethodInfo;
class JniString;
class JniUtils;
#endif // EE_X_ANDROID
} // namespace core

using core::IPlugin;
using core::Library;
using core::Plugin;
using core::PluginManager;

using core::Logger;
using core::LogLevel;

using core::IMessageBridge;
using core::MessageBridge;

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
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_CORE_FWD_HPP */
