#ifndef EE_X_WHEN_ALL_HPP
#define EE_X_WHEN_ALL_HPP

#include <vector>

#include "ee/coroutine/LambdaAwaiter.hpp"
#include "ee/coroutine/NoAwait.hpp"
#include "ee/coroutine/Task.hpp"

namespace ee {
namespace coroutine {
namespace detail {
template <class T>
Task<> toVoidTask(T&& task) {
    co_await task;
    co_return;
}
} // namespace detail

template <class... Args>
Task<> whenAll(Args&&... args) {
    std::vector<Task<>> tasks = {
        detail::toVoidTask(std::forward<Args>(args))...};
    auto counter = std::make_shared<std::size_t>(tasks.size());
    co_await LambdaAwaiter<>([&tasks, counter](auto&& resolve) { //
        for (auto&& task : tasks) {
            noAwait([&task, counter, resolve]() -> Task<> {
                co_await task;
                --(*counter);
                if (*counter == 0) {
                    resolve();
                }
            });
        }
    });
    co_return;
}
} // namespace coroutine
} // namespace ee

#endif // EE_X_WHEN_ALL_HPP
