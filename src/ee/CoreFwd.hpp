//
//  CoreFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_CORE_FWD_HPP
#define EE_X_CORE_FWD_HPP

#include "ee/Macro.hpp"

namespace cppcoro {
class async_mutex;
class async_mutex_lock;
class async_mutex_lock_operation;
class async_mutex_scoped_lock_operation;
} // namespace cppcoro

namespace ee {
namespace core {
struct LogLevel;
class Logger;
class IMessageBridge;
class IService;
class MessageBridge;
class PluginManager;
class ScopeGuard;
class ServiceLocator;
class SpinLock;
class VideoPlayer;
class VideoPlayerManager;

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
struct Task;

template <class T = void>
struct LambdaAwaiter;

using AsyncMutex = cppcoro::async_mutex;
using AsyncMutexLockOperation = cppcoro::async_mutex_lock_operation;
using AsyncMutexScopedLockOperation =
    cppcoro::async_mutex_scoped_lock_operation;

struct SwitchToUiThread;

#if defined(EE_X_ANDROID)
class JniUtils;
#endif // EE_X_ANDROID
} // namespace core

using core::IMessageBridge;
using core::IService;
using core::Logger;
using core::LogLevel;
using core::MessageBridge;
using core::PluginManager;
using core::ScopeGuard;
using core::ServiceLocator;
using core::SpinLock;
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
using core::SwitchToUiThread;
using core::Task;

#if defined(EE_X_ANDROID)
using core::JniUtils;
#endif // EE_X_ANDROID
} // namespace ee

#endif /* EE_X_CORE_FWD_HPP */
