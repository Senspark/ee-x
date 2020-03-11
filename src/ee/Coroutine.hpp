#ifndef EE_X_COROUTINE_HPP
#define EE_X_COROUTINE_HPP

#include <ee/Core.hpp>

#include "ee/CoroutineFwd.hpp"
#include "ee/coroutine/AsyncMutex.hpp"
#include "ee/coroutine/LambdaAwaiter.hpp"
#include "ee/coroutine/NoAwait.hpp"
#include "ee/coroutine/SwitchToUiThread.hpp"
#include "ee/coroutine/Task.hpp"
#include "ee/coroutine/WhenAll.hpp"

#if __has_include("ee/cocos_coroutine/Delay.hpp")
#include "ee/cocos_coroutine/Delay.hpp"
#include "ee/cocos_coroutine/SwitchToCocosThread.hpp"
#endif

#endif // EE_X_COROUTINE_HPP
