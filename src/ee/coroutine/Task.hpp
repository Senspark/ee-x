#ifndef EE_X_TASK_HPP
#define EE_X_TASK_HPP

#include <exception>
#include <experimental/coroutine>
#include <variant>

#include "ee/CoroutineFwd.hpp"

namespace ee {
namespace coroutine {
template <class T>
struct Task {
    struct promise_type {
        std::variant<std::monostate, T, std::exception_ptr> result_;
        std::experimental::coroutine_handle<void> waiter_;

        auto get_return_object() { //
            return Task(this);
        }

        auto initial_suspend() { //
            return std::experimental::suspend_always();
        }

        auto final_suspend() {
            struct Awaiter {
                promise_type* me_;

                bool await_ready() { //
                    return false;
                }

                void await_suspend(
                    std::experimental::coroutine_handle<void> caller) {
                    me_->waiter_.resume();
                }

                void await_resume() {}
            };
            return Awaiter{this};
        }

        template <class U>
        void return_value(U&& u) {
            result_.template emplace<1>(static_cast<U&&>(u));
        }

        void unhandled_exception() {
            result_.template emplace<2>(std::current_exception());
        }
    };

    bool await_ready() { //
        return false;
    }

    void await_suspend(std::experimental::coroutine_handle<void> caller) {
        coro_.promise().waiter_ = caller;
        coro_.resume();
    }

    T await_resume() {
        if (coro_.promise().result_.index() == 2) {
            std::rethrow_exception(std::get<2>(coro_.promise().result_));
        }
        return std::get<1>(coro_.promise().result_);
    }

    Task(const Task&) = delete;

    Task(Task&& other)
        : coro_(other.coro_) {
        other.coro_ = nullptr;
    }

    ~Task() {
        if (coro_) { //
            coro_.destroy();
        }
    }

private:
    using Handle = std::experimental::coroutine_handle<promise_type>;

    Task(promise_type* p)
        : coro_(Handle::from_promise(*p)) {}

    Handle coro_;
};

template <>
struct Task<void> {
    struct promise_type {
        std::variant<std::monostate, std::exception_ptr> result_;
        std::experimental::coroutine_handle<void> waiter_;

        auto get_return_object() { //
            return Task(this);
        }

        auto initial_suspend() { //
            return std::experimental::suspend_always();
        }

        auto final_suspend() {
            struct Awaiter {
                promise_type* me_;

                bool await_ready() { //
                    return false;
                }

                void await_suspend(
                    std::experimental::coroutine_handle<void> caller) {
                    me_->waiter_.resume();
                }

                void await_resume() {}
            };
            return Awaiter{this};
        }

        void return_void() {}

        void unhandled_exception() {
            result_.template emplace<1>(std::current_exception());
        }
    };

    bool await_ready() { //
        return false;
    }

    void await_suspend(std::experimental::coroutine_handle<void> caller) {
        coro_.promise().waiter_ = caller;
        coro_.resume();
    }

    void await_resume() {
        if (coro_.promise().result_.index() == 1) {
            std::rethrow_exception(std::get<1>(coro_.promise().result_));
        }
    }

    Task(const Task&) = delete;

    Task(Task&& other)
        : coro_(other.coro_) {
        other.coro_ = nullptr;
    }

    ~Task() {
        if (coro_) { //
            coro_.destroy();
        }
    }

private:
    using Handle = std::experimental::coroutine_handle<promise_type>;

    explicit Task(promise_type* p)
        : coro_(Handle::from_promise(*p)) {}

    Handle coro_;
};
} // namespace coroutine
} // namespace ee

#endif // EE_X_TASK_HPP
