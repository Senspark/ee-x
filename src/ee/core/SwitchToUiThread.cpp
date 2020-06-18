#include "ee/core/SwitchToUiThread.hpp"

#include "ee/core/Thread.hpp"

namespace ee {
namespace core {
using Self = SwitchToUiThread;

Self::SwitchToUiThread() = default;
Self::~SwitchToUiThread() = default;

void Self::await_suspend(std::experimental::coroutine_handle<> handle) {
    runOnMainThread([this, handle]() mutable { //
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
