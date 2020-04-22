#include "ee/cocos/SwitchToCocosThread.hpp"

#include <base/CCDirector.h>
#include <base/CCScheduler.h>

namespace ee {
namespace cocos {
using Self = SwitchToCocosThread;

Self::SwitchToCocosThread()
    : ready_(false) {}

Self::~SwitchToCocosThread() = default;

void Self::await_suspend(std::experimental::coroutine_handle<> handle) {
    auto director = cocos2d::Director::getInstance();
    auto scheduler = director->getScheduler();
    scheduler->performFunctionInCocosThread([this, handle]() mutable {
        ready_ = true;
        handle.resume();
    });
}

bool Self::await_ready() {
    return ready_;
}

void Self::await_resume() {}
} // namespace cocos
} // namespace ee
