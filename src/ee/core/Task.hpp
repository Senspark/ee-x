#ifndef EE_X_TASK_HPP
#define EE_X_TASK_HPP

#ifdef __cplusplus

#include <exception>
#include <experimental/coroutine>
#include <variant>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
template <class T>
class Task {
public:
    class promise_type {
    public:
        auto get_return_object() { //
            return Task(this);
        }

        auto initial_suspend() { //
            return std::experimental::suspend_always();
        }

        auto final_suspend() {
            struct Awaiter {
                promise_type* me_;

                bool await_ready() const noexcept { //
                    return false;
                }

                void
                await_suspend(std::experimental::coroutine_handle<> handle) {
                    me_->handle_.resume();
                }

                void await_resume() noexcept {}
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

    private:
        friend Task;

        std::variant<std::monostate, T, std::exception_ptr> result_;
        std::experimental::coroutine_handle<> handle_;
    };

    Task(const Task&) = delete;

    Task(Task&& other)
        : handle_(std::exchange(other.handle_, nullptr)) {}

    ~Task() {
        if (handle_) {
            handle_.destroy();
        }
    }

    bool await_ready() const noexcept { //
        return false;
    }

    void await_suspend(std::experimental::coroutine_handle<void> handle) {
        handle_.promise().handle_ = handle;
        handle_.resume();
    }

    T await_resume() {
        if (handle_.promise().result_.index() == 2) {
            std::rethrow_exception(std::get<2>(handle_.promise().result_));
        }
        return std::get<1>(handle_.promise().result_);
    }

private:
    using Handle = std::experimental::coroutine_handle<promise_type>;

    Task(promise_type* p)
        : handle_(Handle::from_promise(*p)) {}

    Handle handle_;
};

template <>
class Task<void> {
public:
    class promise_type {
    public:
        auto get_return_object() { //
            return Task(this);
        }

        auto initial_suspend() { //
            return std::experimental::suspend_always();
        }

        auto final_suspend() {
            struct Awaiter {
                promise_type* me_;

                bool await_ready() const noexcept { //
                    return false;
                }

                void
                await_suspend(std::experimental::coroutine_handle<> handle) {
                    me_->handle_.resume();
                }

                void await_resume() noexcept {}
            };
            return Awaiter{this};
        }

        void return_void() noexcept {}

        void unhandled_exception() {
            result_.template emplace<1>(std::current_exception());
        }

    private:
        friend Task;

        std::variant<std::monostate, std::exception_ptr> result_;
        std::experimental::coroutine_handle<> handle_;
    };

    Task(const Task&) = delete;

    Task(Task&& other)
        : handle_(std::exchange(other.handle_, nullptr)) {}

    ~Task() {
        if (handle_) {
            handle_.destroy();
        }
    }

    bool await_ready() const noexcept { //
        return false;
    }

    void await_suspend(std::experimental::coroutine_handle<void> handle) {
        handle_.promise().handle_ = handle;
        handle_.resume();
    }

    void await_resume() {
        if (handle_.promise().result_.index() == 1) {
            std::rethrow_exception(std::get<1>(handle_.promise().result_));
        }
    }

private:
    using Handle = std::experimental::coroutine_handle<promise_type>;

    explicit Task(promise_type* p)
        : handle_(Handle::from_promise(*p)) {}

    Handle handle_;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif // EE_X_TASK_HPP
