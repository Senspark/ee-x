#include "ee/coroutine/SwitchToUiThread.hpp"

#include <ee/core/Utils.hpp>

namespace ee {
namespace coroutine {
using Self = SwitchToUiThread;

Self::SwitchToUiThread()
    : ready_(false) {}

Self::~SwitchToUiThread() = default;

void Self::await_suspend(std::experimental::coroutine_handle<> handle) {
    runOnUiThread([this, handle]() mutable {
        ready_ = true;
        handle.resume();
    });
}

bool Self::await_ready() {
    return ready_;
}

void Self::await_resume() {}
} // namespace coroutine
} // namespace ee
