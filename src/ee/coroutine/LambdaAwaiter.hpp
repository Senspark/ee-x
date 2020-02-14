#ifndef EE_X_LAMBDA_AWAITER_HPP
#define EE_X_LAMBDA_AWAITER_HPP

#include <experimental/coroutine>
#include <functional>

#include "ee/CoroutineFwd.hpp"

namespace ee {
namespace coroutine {
template <class Result>
struct LambdaAwaiter {
public:
    using Resolve = std::function<void(Result result)>;
    using Function = std::function<void(const Resolve& resolve)>;

    explicit LambdaAwaiter(const Function& f)
        : f_(f)
        , ready_(false) {}

    void await_suspend(std::experimental::coroutine_handle<> handle) {
        f_([this, handle](Result result) mutable {
            ready_ = true;
            result_ = result;
            handle.resume();
        });
    }

    bool await_ready() { //
        return ready_;
    }

    Result await_resume() { //
        return result_;
    }

private:
    Function f_;
    bool ready_;
    Result result_;
};

template <>
struct LambdaAwaiter<void> {
    using Resolve = std::function<void()>;
    using Function = std::function<void(const Resolve& resolve)>;

    explicit LambdaAwaiter(const Function& f)
        : f_(f)
        , ready_(false) {}

    void await_suspend(std::experimental::coroutine_handle<> handle) {
        f_([this, handle]() mutable {
            ready_ = true;
            handle.resume();
        });
    }

    bool await_ready() { //
        return ready_;
    }

    void await_resume() {}

private:
    Function f_;
    bool ready_;
};
} // namespace coroutine
} // namespace ee

#endif // EE_X_LAMBDA_AWAITER_HPP
