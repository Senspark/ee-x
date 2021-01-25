#include "ee/cocos/SwitchToCocosThread.hpp"

#ifdef EE_X_COCOS_CPP
#include <base/CCDirector.h>
#include <base/CCScheduler.h>

namespace ee {
namespace cocos {
using Self = SwitchToCocosThread;

Self::SwitchToCocosThread() = default;
Self::~SwitchToCocosThread() = default;

void Self::await_suspend(std::experimental::coroutine_handle<> handle) {
    auto director = cocos2d::Director::getInstance();
    auto scheduler = director->getScheduler();
    scheduler->performFunctionInCocosThread([this, handle]() mutable { //
        handle.resume();
    });
}

bool Self::await_ready() {
    // Always suspend.
    return false;
}

void Self::await_resume() {}
} // namespace cocos
} // namespace ee

#endif // EE_X_COCOS_CPP