#include "ee/core/Delay.hpp"

#include <ee/cocos/CocosFwd.hpp>
#include <base/CCScheduler.h>

#ifdef EE_X_COCOS_CPP
#include <base/CCDirector.h>
#else // EE_X_COCOS_CPP
#include <platform/CCApplication.h>
#endif // EE_X_COCOS_CPP

namespace ee {
namespace core {
using Self = Delay;

Self::Delay(float duration)
    : duration_(duration)
    , ready_(false) {
    CC_ASSERT(duration_ >= 0);
}

void Self::await_suspend(coroutine_handle handle) {
    if (duration_ == 0) {
        handle.resume();
        return;
    }
#ifdef EE_X_COCOS_CPP
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
#else // EE_X_COCOS_CPP
    auto scheduler = cocos2d::Application::getInstance()->getScheduler();
#endif // EE_X_COCOS_CPP
    scheduler->schedule(
        [this, handle](float delta) mutable {
            ready_ = true;
            handle.resume();
        },
        this, 0, 0, duration_, false, "await_suspend");
}

bool Self::await_ready() {
    if (duration_ == 0) {
        return true;
    }
    return ready_;
}

void Self::await_resume() {}
} // namespace core
} // namespace ee
