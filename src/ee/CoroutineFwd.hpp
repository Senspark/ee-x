#ifndef EE_X_COROUTINE_FWD_HPP
#define EE_X_COROUTINE_FWD_HPP

namespace ee {
namespace coroutine {
template <class T = void>
struct Task;

template <class T = void>
struct LambdaAwaiter;
} // namespace coroutine

using coroutine::Task;
using coroutine::LambdaAwaiter;
} // namespace ee

#endif // EE_X_COROUTINE_FWD_HPP