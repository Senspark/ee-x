#ifndef EE_X_NO_AWAIT_HPP
#define EE_X_NO_AWAIT_HPP

#ifdef __cplusplus

#include <experimental/coroutine>
#include <type_traits>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
/// https://devblogs.microsoft.com/oldnewthing/20190322-00/?p=102354
class CompleteAsynchronously {
public:
    class promise_type {
    public:
        auto get_return_object() { //
            return CompleteAsynchronously();
        }

        auto initial_suspend() const noexcept { //
            return std::experimental::suspend_never();
        }

        auto final_suspend() const noexcept { //
            return std::experimental::suspend_never();
        }

        void return_void() const noexcept {}

        void unhandled_exception() noexcept { //
            std::terminate();
        }
    };
};

namespace detail {
template <class T>
struct IsTask : std::false_type {};

template <class T>
struct IsTask<Task<T>> : std::true_type {};
} // namespace detail

template <class T, class = std::enable_if_t<std::is_invocable_v<T> ||
                                            detail::IsTask<T>::value>>
CompleteAsynchronously noAwait(T&& value) {
    if constexpr (std::is_invocable_v<T>) {
        auto lambda = std::forward<T>(value);
        co_await lambda();
    }
    if constexpr (detail::IsTask<T>::value) {
        auto task = std::forward<T>(value);
        co_await task;
    }
}
} // namespace core

using core::noAwait;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_NO_AWAIT_HPP
