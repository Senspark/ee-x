#ifndef EE_X_MAKE_AWAITER_HPP
#define EE_X_MAKE_AWAITER_HPP

#ifdef __cplusplus

#include "ee/core/NoAwait.hpp"
#include "ee/core/Task.hpp"

namespace ee {
namespace core {
/// Note: Generic lambda causes crash (xcode 11.3 + ndk r21).
template <class T>
struct MakeAwaiter;

template <class T, class R, class... Args>
struct MakeAwaiter<R (T::*)(Args...) const> {
    static auto make(const std::function<R(Args...)>& f) {
        return [f](Args... args) {
            noAwait([f, args...]() -> Task<> { //
                co_await f(args...);
            });
        };
    }
};

template <class Function>
auto makeAwaiter(const Function& f) {
    return MakeAwaiter<decltype(&Function::operator())>::make(f);
}
} // namespace core

using core::makeAwaiter;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_MAKE_AWAITER_HPP
