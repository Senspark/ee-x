#ifndef EE_X_SWITCH_TO_MAIN_THREAD_HPP
#define EE_X_SWITCH_TO_MAIN_THREAD_HPP

#ifdef __cplusplus

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
class SwitchToMainThread {
private:
    using Self = SwitchToMainThread;

public:
    SwitchToMainThread();
    ~SwitchToMainThread();

    SwitchToMainThread(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    void await_suspend(coroutine_handle handle);
    bool await_ready();
    void await_resume();
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif // EE_X_SWITCH_TO_MAIN_THREAD_HPP
