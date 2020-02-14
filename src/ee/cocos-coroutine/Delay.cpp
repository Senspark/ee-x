#include "ee/cocos-coroutine/Delay.hpp"

#include <base/CCDirector.h>
#include <base/CCScheduler.h>

namespace ee {
namespace coroutine {
using Self = Delay;

Self::Delay(float duration)
    : duration_(duration) {
    CC_ASSERT(duration_ >= 0);
}

void Self::await_suspend(std::experimental::coroutine_handle<> handle) {
    if (duration_ == 0) {
        handle.resume();
        return;
    }
    auto director = cocos2d::Director::getInstance();
    auto scheduler = director->getScheduler();
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
} // namespace coroutine
} // namespace ee
