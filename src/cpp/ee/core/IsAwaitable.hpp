#ifndef EE_X_IS_AWAITABLE_HPP
#define EE_X_IS_AWAITABLE_HPP

#ifdef __cplusplus

#include <type_traits>

#include "ee/core/CoreFwd.hpp"

namespace cppcoro {
namespace detail {
// Helper type that can be cast-to from any type.
struct any {
    template <typename T>
    any(T&&) noexcept {}
};

template <typename T>
struct is_coroutine_handle : std::false_type {};

template <typename PROMISE>
struct is_coroutine_handle<estd::coroutine_handle<PROMISE>>
    : std::true_type {};

// NOTE: We're accepting a return value of coroutine_handle<P> here
// which is an extension supported by Clang which is not yet part of
// the C++ coroutines TS.
template <typename T>
struct is_valid_await_suspend_return_value
    : std::disjunction<std::is_void<T>, std::is_same<T, bool>,
                       is_coroutine_handle<T>> {};

template <typename T, typename = std::void_t<>>
struct is_awaiter : std::false_type {};

// NOTE: We're testing whether await_suspend() will be callable using an
// arbitrary coroutine_handle here by checking if it supports being passed
// a coroutine_handle<void>. This may result in a false-result for some
// types which are only awaitable within a certain context.
template <typename T>
struct is_awaiter<
    T, std::void_t<decltype(std::declval<T>().await_ready()),
                   decltype(std::declval<T>().await_suspend(
                       std::declval<estd::coroutine_handle<>>())),
                   decltype(std::declval<T>().await_resume())>>
    : std::conjunction<
          std::is_constructible<bool,
                                decltype(std::declval<T>().await_ready())>,
          detail::is_valid_await_suspend_return_value<decltype(
              std::declval<T>().await_suspend(
                  std::declval<estd::coroutine_handle<>>()))>> {};

template <typename T>
auto get_awaiter_impl(T&& value, int) noexcept(
    noexcept(static_cast<T&&>(value).operator co_await()))
    -> decltype(static_cast<T&&>(value).operator co_await()) {
    return static_cast<T&&>(value).operator co_await();
}

template <typename T>
auto get_awaiter_impl(T&& value, long) noexcept(
    noexcept(operator co_await(static_cast<T&&>(value))))
    -> decltype(operator co_await(static_cast<T&&>(value))) {
    return operator co_await(static_cast<T&&>(value));
}

template <typename T,
          std::enable_if_t<cppcoro::detail::is_awaiter<T&&>::value, int> = 0>
T&& get_awaiter_impl(T&& value, cppcoro::detail::any) noexcept {
    return static_cast<T&&>(value);
}

template <typename T>
auto get_awaiter(T&& value) noexcept(
    noexcept(detail::get_awaiter_impl(static_cast<T&&>(value), 123)))
    -> decltype(detail::get_awaiter_impl(static_cast<T&&>(value), 123)) {
    return detail::get_awaiter_impl(static_cast<T&&>(value), 123);
}
} // namespace detail

template <typename T, typename = std::void_t<>>
struct is_awaitable : std::false_type {};

template <typename T>
struct is_awaitable<
    T, std::void_t<decltype(cppcoro::detail::get_awaiter(std::declval<T>()))>>
    : std::true_type {};

template <typename T>
constexpr bool is_awaitable_v = is_awaitable<T>::value;
} // namespace cppcoro

namespace ee {
namespace core {
template <class T>
constexpr bool IsAwaitableV = cppcoro::is_awaitable_v<T>;
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif // EE_X_IS_AWAITABLE_HPP
