#ifndef EE_X_DELAY_HPP
#define EE_X_DELAY_HPP

#include <experimental/coroutine>

namespace ee {
namespace coroutine {
struct Delay {
public:
    explicit Delay(float duration);

    void await_suspend(std::experimental::coroutine_handle<> handle);
    bool await_ready();
    void await_resume();

private:
    float duration_;
    bool ready_;
};
} // namespace coroutine
} // namespace ee

#endif // EE_X_DELAY_HPP
