//
//  ScopeGuard.hpp
//  ee-library
//
//  Created by Zinge on 4/13/17.
//
//

#ifndef EE_LIBRARY_SCOPE_GUARD_HPP_
#define EE_LIBRARY_SCOPE_GUARD_HPP_

#include <functional>

namespace ee {
/// http://stackoverflow.com/questions/10270328/the-simplest-and-neatest-c11-scopeguard
class ScopeGuard {
public:
    template <class Callable>
    explicit ScopeGuard(Callable&& undoFunction)
        : f_(std::forward<Callable>(undoFunction)) {}

    ScopeGuard(ScopeGuard&& other)
        : f_(std::move(other.f_)) {
        other.f_ = nullptr;
    }

    ScopeGuard& operator=(ScopeGuard&& other) {
        f_ = std::move(other.f_);
        other.f_ = nullptr;
        return *this;
    }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    ~ScopeGuard() {
        if (f_) {
            f_();
        }
    }

    void invoke() {
        f_();
        dismiss();
    }

    void dismiss() noexcept { f_ = nullptr; }

private:
    std::function<void()> f_;
};
} // namespace ee

#endif /* EE_LIBRARY_SCOPE_GUARD_HPP_ */
