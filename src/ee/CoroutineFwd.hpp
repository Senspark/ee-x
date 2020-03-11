#ifndef EE_X_COROUTINE_FWD_HPP
#define EE_X_COROUTINE_FWD_HPP

#include <ee/CoreFwd.hpp>

namespace cppcoro {
class async_mutex;
class async_mutex_lock;
class async_mutex_lock_operation;
class async_mutex_scoped_lock_operation;
} // namespace cppcoro

namespace ee {
namespace coroutine {
template <class T = void>
struct Task;

template <class T = void>
struct LambdaAwaiter;

using AsyncMutex = cppcoro::async_mutex;
using AsyncMutexLockOperation = cppcoro::async_mutex_lock_operation;
using AsyncMutexScopedLockOperation =
    cppcoro::async_mutex_scoped_lock_operation;

struct Delay;
struct SwitchToCocosThread;
struct SwitchToUiThread;
} // namespace coroutine

using coroutine::AsyncMutex;
using coroutine::AsyncMutexLockOperation;
using coroutine::AsyncMutexScopedLockOperation;
using coroutine::Delay;
using coroutine::LambdaAwaiter;
using coroutine::SwitchToCocosThread;
using coroutine::SwitchToUiThread;
using coroutine::Task;
} // namespace ee

#endif // EE_X_COROUTINE_FWD_HPP
