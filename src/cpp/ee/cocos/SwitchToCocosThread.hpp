#ifndef EE_X_SWITCH_TO_COCOS_THREAD_HPP
#define EE_X_SWITCH_TO_COCOS_THREAD_HPP

#ifdef __cplusplus

#include "ee/cocos/CocosFwd.hpp"

namespace ee {
namespace cocos {
class SwitchToCocosThread {
private:
    using Self = SwitchToCocosThread;

public:
    SwitchToCocosThread();
    ~SwitchToCocosThread();

    SwitchToCocosThread(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    void await_suspend(estd::coroutine_handle<> handle);
    bool await_ready();
    void await_resume();
};
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif // EE_X_SWITCH_TO_COCOS_THREAD_HPP
