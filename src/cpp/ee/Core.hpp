//
//  Core.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_CORE_HPP
#define EE_X_CORE_HPP

#ifdef __cplusplus

#include "ee/CoreFwd.hpp"
#include "ee/core/AsyncMutex.hpp"
#include "ee/core/DynamicValue.hpp"
#include "ee/core/IMessageBridge.hpp"
#include "ee/core/IService.hpp"
#include "ee/core/LambdaAwaiter.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/MakeAwaiter.hpp"
#include "ee/core/NoAwait.hpp"
#include "ee/core/ObserverBinder.hpp"
#include "ee/core/ObserverHandle.hpp"
#include "ee/core/ObserverManager.hpp"
#include "ee/core/Platform.hpp"
#include "ee/core/PluginManager.hpp"
#include "ee/core/Random.hpp"
#include "ee/core/SafeObserverManager.hpp"
#include "ee/core/ScopeGuard.hpp"
#include "ee/core/ServiceLocator.hpp"
#include "ee/core/SpinLock.hpp"
#include "ee/core/SwitchToLibraryThread.hpp"
#include "ee/core/SwitchToMainThread.hpp"
#include "ee/core/Task.hpp"
#include "ee/core/Thread.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/VideoPlayer.hpp"
#include "ee/core/VideoPlayerManager.hpp"
#include "ee/core/WhenAll.hpp"

#endif // __cplusplus

#endif /* EE_X_CORE_HPP */
