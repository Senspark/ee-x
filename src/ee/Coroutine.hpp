#ifndef EE_X_COROUTINE_HPP
#define EE_X_COROUTINE_HPP

#include <ee/Core.hpp>

#include "ee/CoroutineFwd.hpp"
#include "ee/coroutine/LambdaAwaiter.hpp"
#include "ee/coroutine/NoAwait.hpp"
#include "ee/coroutine/Task.hpp"

#if __has_include("ee/cocos-coroutine/Delay.hpp")
#include "ee/cocos_coroutine/Delay.hpp"
#endif

#endif // EE_X_COROUTINE_HPP
