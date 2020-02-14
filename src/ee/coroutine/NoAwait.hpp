#ifndef EE_X_NO_AWAIT_HPP
#define EE_X_NO_AWAIT_HPP

#include <experimental/coroutine>
#include <type_traits>

#include "ee/CoroutineFwd.hpp"

namespace ee {
namespace coroutine {
/// https://devblogs.microsoft.com/oldnewthing/20190322-00/?p=102354
struct CompleteAsynchronously {
    struct promise_type {
        auto get_return_object() const noexcept {
            return CompleteAsynchronously();
        }

        void return_void() const noexcept {}

        auto initial_suspend() const noexcept { //
            return std::experimental::suspend_never();
        }

        auto final_suspend() const noexcept { //
            return std::experimental::suspend_never();
        }

        void unhandled_exception() noexcept { //
            std::terminate();
        }
    };
};

template <class T>
CompleteAsynchronously NoAwait(T task) {
    if constexpr (std::is_invocable_v<T>) {
        co_await task();
    } else {
        co_await task;
    }
}
} // namespace coroutine

using coroutine::NoAwait;
} // namespace ee

#endif // EE_X_NO_AWAIT_HPP
