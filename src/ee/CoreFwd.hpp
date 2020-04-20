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

#if defined(EE_X_ANDROID)
using core::JniUtils;
#endif // EE_X_ANDROID
} // namespace ee

#endif /* EE_X_CORE_FWD_HPP */
