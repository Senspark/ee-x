#include "ee/core/SwitchToUiThread.hpp"

#include "ee/core/Thread.hpp"

namespace ee {
namespace core {
using Self = SwitchToUiThread;

Self::SwitchToUiThread()
    : ready_(false) {}

Self::~SwitchToUiThread() = default;

void Self::await_suspend(std::experimental::coroutine_handle<> handle) {
    runOnMainThread([this, handle]() mutable {
        ready_ = true;
        handle.resume();
    });
}

bool Self::await_ready() {
    return ready_;
}

void Self::await_resume() {}
} // namespace core
} // namespace ee
