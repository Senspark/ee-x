//
//  SwitchToLibraryThread.cpp
//  Pods
//
//  Created by eps on 7/15/20.
//

#include "ee/core/SwitchToLibraryThread.hpp"

#include "ee/core/Thread.hpp"

namespace ee {
namespace core {
using Self = SwitchToLibraryThread;

Self::SwitchToLibraryThread() = default;
Self::~SwitchToLibraryThread() = default;

void Self::await_suspend(estd::coroutine_handle<> handle) {
    Thread::runOnLibraryThread([handle]() mutable { //
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
