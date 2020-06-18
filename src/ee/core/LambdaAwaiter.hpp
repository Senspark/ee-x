#ifndef EE_X_LAMBDA_AWAITER_HPP
#define EE_X_LAMBDA_AWAITER_HPP

#ifdef __cplusplus

#include <experimental/coroutine>
#include <functional>
#include <mutex>
#include <optional>
#include <vector>

#include "ee/core/IsAwaitable.hpp"
#include "ee/core/NoAwait.hpp"
#include "ee/core/SpinLock.hpp"
#include "ee/core/Task.hpp"

namespace ee {
namespace core {
namespace detail {
template <class Resolver, class Function, class Callable>
Function makeFunction(const Callable& callable) {
    if constexpr (IsAwaitableV<
                      std::invoke_result_t<Callable, const Resolve&>>) {
        return [callable](const Resolve& resolve) {
            noAwait([callable, resolve]() -> Task<> { //
                co_await callable(resolve);
            });
        };
    }
    // Normal callable.
    return callable;
}
} // namespace detail

template <class Result>
class LambdaAwaiter {
public:
    using Resolver = std::function<void(Result result)>;
    using Function = std::function<void(const Resolver& resolver)>;

    template <class Callable>
    explicit LambdaAwaiter(const Callable& callable)
        : f_(detail::makeFunction<Resolver, Function>(callable))
        , invoked_(false)
        , ready_(false) {
        resolver_ = [this](Result result) mutable {
            std::unique_lock<SpinLock> lk(lock_);
            ready_ = true;
            result_ = result;
            auto handles = handles_;
            lk.unlock();

            for (auto&& handle : handles) {
                handle.resume();
            }
        };
    }

    LambdaAwaiter(const LambdaAwaiter&) = delete;

    LambdaAwaiter(LambdaAwaiter&& other)
        : f_(std::exchange(other.f_, nullptr))
        , resolver_(std::exchange(other.resolver_, nullptr))
        , result_(std::exchange(other.result_, std::nullopt))
        , invoked_(std::exchange(other.invoked_, false))
        , ready_(std::exchange(other.ready_, false))
        , handles_(std::exchange(other.handles_, {})) {}

    void await_suspend(std::experimental::coroutine_handle<> handle) {
        std::unique_lock<SpinLock> lk(lock_);
        handles_.push_back(handle);
        if (invoked_) {
            // Waiting.
            return;
        }
        invoked_ = true;
        lk.unlock();

        f_(resolver_);
    }

    bool await_ready() {
        std::scoped_lock<SpinLock> lk(lock_);
        return ready_;
    }

    Result await_resume() {
        std::scoped_lock<SpinLock> lk(lock_);
        return result_.value();
    }

private:
    Function f_;
    Resolver resolver_;
    SpinLock lock_;
    bool invoked_;
    bool ready_;
    std::optional<Result> result_;
    std::vector<std::experimental::coroutine_handle<>> handles_;
};

template <>
class LambdaAwaiter<void> {
public:
    using Resolver = std::function<void()>;
    using Function = std::function<void(const Resolver& resolve)>;

    template <class Callable>
    explicit LambdaAwaiter(const Callable& callable)
        : f_(detail::makeFunction<Resolver, Function>(callable))
        , invoked_(false)
        , ready_(false) {
        resolver_ = [this]() mutable {
            std::unique_lock<SpinLock> lk(lock_);
            ready_ = true;
            auto handles = handles_;
            lk.unlock();

            for (auto&& handle : handles) {
                handle.resume();
            }
        };
    }

    LambdaAwaiter(const LambdaAwaiter&) = delete;

    LambdaAwaiter(LambdaAwaiter&& other)
        : f_(std::exchange(other.f_, nullptr))
        , resolver_(std::exchange(other.resolver_, nullptr))
        , invoked_(std::exchange(other.invoked_, false))
        , ready_(std::exchange(other.ready_, false))
        , handles_(std::exchange(other.handles_, {})) {}

    void await_suspend(std::experimental::coroutine_handle<> handle) {
        std::unique_lock<SpinLock> lk(lock_);
        handles_.push_back(handle);
        if (invoked_) {
            // Waiting.
            return;
        }
        invoked_ = true;
        lk.unlock();

        f_(resolver_);
    }

    bool await_ready() {
        std::scoped_lock<SpinLock> lk(lock_);
        return ready_;
    }

    void await_resume() {}

private:
    Function f_;
    Resolver resolver_;
    SpinLock lock_;
    bool invoked_;
    bool ready_;
    std::vector<std::experimental::coroutine_handle<>> handles_;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif // EE_X_LAMBDA_AWAITER_HPP
