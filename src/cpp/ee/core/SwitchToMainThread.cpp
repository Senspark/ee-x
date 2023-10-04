#include "ee/core/SwitchToMainThread.hpp"
#include "ee/core/Thread.hpp"

namespace ee {
namespace core {
using Self = SwitchToMainThread;

Self::SwitchToMainThread() = default;
Self::~SwitchToMainThread() = default;

void Self::await_suspend(coroutine_handle handle) {
    runOnMainThread([handle]() mutable { //
        handle.resume();
    });
}

bool Self::await_ready() {
    // Always suspend.
    return false;
}

void Self::await_resume() {}
} // namespace core
} // namespace ee
