#ifndef EE_X_WHEN_ALL_HPP
#define EE_X_WHEN_ALL_HPP

#ifdef __cplusplus

#include "ee/core/LambdaAwaiter.hpp"
#include "ee/core/NoAwait.hpp"
#include "ee/core/Task.hpp"

namespace ee {
namespace core {
namespace detail {
template <class T>
void whenAllHelper(const std::shared_ptr<int>& counter,
             const std::function<void()>& resolve, T&& callable) {
    noAwait([counter, resolve, callable]() -> Task<> {
        co_await callable();
        --(*counter);
        if (*counter == 0) {
            resolve();
        }
    });
}
} // namespace detail

template <class... Args>
Task<> whenAll(Args&&... args) {
    auto callableTuple = std::forward_as_tuple(std::forward<Args>(args)...);
    auto counter = std::make_shared<int>(sizeof...(Args));
    co_await LambdaAwaiter<>([&callableTuple, counter](auto&& resolve) {
        std::apply(
            [counter, resolve](auto&&... callables) {
                ((detail::whenAllHelper(counter, resolve, callables)), ...);
            },
            callableTuple);
    });
}
} // namespace core

using core::whenAll;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_WHEN_ALL_HPP
