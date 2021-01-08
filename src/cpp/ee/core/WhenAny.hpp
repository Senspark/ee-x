//
//  WhenAny.hpp
//  ee-x-d542b565
//
//  Created by eps on 1/8/21.
//

#ifndef EE_X_WHEN_ANY_HPP
#define EE_X_WHEN_ANY_HPP

#ifdef __cplusplus

#include "ee/core/LambdaAwaiter.hpp"
#include "ee/core/NoAwait.hpp"
#include "ee/core/Task.hpp"

namespace ee {
namespace core {
namespace detail {
template <class T>
void whenAnyHelper(const std::shared_ptr<bool>& flag,
                   const std::function<void()>& resolve, T&& callable) {
    noAwait([flag, resolve, callable]() -> Task<> {
        co_await callable();
        if (*flag) {
            *flag = false;
            resolve();
        }
    });
}
} // namespace detail

template <class... Args>
Task<> whenAny(Args&&... args) {
    auto callableTuple = std::forward_as_tuple(std::forward<Args>(args)...);
    auto flag = std::make_shared<bool>(true);
    co_await LambdaAwaiter<>([&callableTuple, flag](auto&& resolve) {
        std::apply(
            [flag, resolve](auto&&... callables) {
                ((detail::whenAnyHelper(flag, resolve, callables)), ...);
            },
            callableTuple);
    });
}
} // namespace core

using core::whenAny;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_WHEN_ANY_HPP */
