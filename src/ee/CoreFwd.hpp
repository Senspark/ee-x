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
class Metrics;
class IMessageBridge;
class IService;
class MessageBridge;
class PluginManager;
class ServiceLocator;
class SpinLock;
class VideoPlayer;
class VideoPlayerManager;

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
using core::Metrics;
using core::PluginManager;
using core::ServiceLocator;
using core::VideoPlayer;
using core::VideoPlayerManager;

using core::IObserverManager;
using core::ObserverManager;

#if defined(EE_X_ANDROID)
using core::JniUtils;
#endif // EE_X_ANDROID
} // namespace ee

#endif /* EE_X_CORE_FWD_HPP */
