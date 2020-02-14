#ifndef EE_X_COROUTINE_FWD_HPP
#define EE_X_COROUTINE_FWD_HPP

namespace ee {
namespace coroutine {
template <class T = void>
struct Task;

template <class T = void>
struct LambdaAwaiter;

struct Delay;
} // namespace coroutine

using coroutine::Delay;
using coroutine::LambdaAwaiter;
using coroutine::Task;
} // namespace ee

#endif // EE_X_COROUTINE_FWD_HPP