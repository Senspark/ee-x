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
class MessageBridge;
class SpinLock;
class VideoPlayer;
class VideoPlayerManager;

#if defined(EE_X_ANDROID)
class JniUtils;
#endif // EE_X_ANDROID
} // namespace core

using core::LogLevel;
using core::Logger;
using core::Metrics;
using core::IMessageBridge;
using core::MessageBridge;
using core::VideoPlayer;
using core::VideoPlayerManager;

#if defined(EE_X_ANDROID)
using core::JniUtils;
#endif // EE_X_ANDROID
} // namespace ee

#endif /* EE_X_CORE_FWD_HPP */
