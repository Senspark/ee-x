#ifndef EE_X_WHEN_ALL_HPP
#define EE_X_WHEN_ALL_HPP

#ifdef __cplusplus

#include <vector>

#include "ee/core/LambdaAwaiter.hpp"
#include "ee/core/NoAwait.hpp"
#include "ee/core/Task.hpp"

namespace ee {
namespace core {
template <class... Args>
Task<> whenAll(Args&&... args) {
    auto taskTuple = std::forward_as_tuple(std::forward<Args>(args)...);
    auto counter = std::make_shared<int>(sizeof...(Args));
    co_await LambdaAwaiter<>([&taskTuple, counter](auto&& resolve) {
        std::apply(
            [counter, resolve](auto&&... taskList) {
                (([counter, resolve](auto&& task) {
                     noAwait([counter, resolve,
                              t = std::move(task)]() mutable -> Task<> {
                         co_await t;
                         --(*counter);
                         if (*counter == 0) {
                             resolve();
                         }
                     });
                 }(taskList)),
                 ...);
            },
            taskTuple);
    });
}
} // namespace core

using core::whenAll;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_WHEN_ALL_HPP
