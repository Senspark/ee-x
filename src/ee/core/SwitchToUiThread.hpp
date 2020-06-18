#ifndef EE_X_SWITCH_TO_UI_THREAD_HPP
#define EE_X_SWITCH_TO_UI_THREAD_HPP

#ifdef __cplusplus

#include <experimental/coroutine>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
struct SwitchToUiThread {
private:
    using Self = SwitchToUiThread;

public:
    SwitchToUiThread();
    ~SwitchToUiThread();

    SwitchToUiThread(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    void await_suspend(std::experimental::coroutine_handle<> handle);
    bool await_ready();
    void await_resume();
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif // EE_X_SWITCH_TO_UI_THREAD_HPP
