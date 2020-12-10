#ifndef EE_X_DELAY_HPP
#define EE_X_DELAY_HPP

#ifdef __cplusplus

#include <experimental/coroutine>

#include "ee/CocosFwd.hpp"

namespace ee {
namespace cocos {
class Delay {
private:
    using Self = Delay;

public:
    explicit Delay(float duration);

    Delay(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    void await_suspend(std::experimental::coroutine_handle<> handle);
    bool await_ready();
    void await_resume();

private:
    float duration_;
    bool ready_;
};
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif // EE_X_DELAY_HPP
