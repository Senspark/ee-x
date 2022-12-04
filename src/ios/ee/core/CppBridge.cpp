#include <functional>
#include <string>

#if __has_include("EEMessageBridge.h")
// Build dynamic framework.
#include "EEMessageBridge.h"
#include "EEThread.h"
#else
#include "ee/core/EEMessageBridge.h"
#include "ee/core/EEThread.h"
#endif

using CallCppCallback =
    std::function<void(const std::string& tag, const std::string& message)>;
using RunOnMainThreadCallback = std::function<void()>;
using RunOnMainThreadDelayedCallback = std::function<void(int key)>;

namespace {
CallCppCallback callCppCallback_;
RunOnMainThreadCallback runOnMainThreadCallback_;
RunOnMainThreadDelayedCallback runOnMainThreadDelayedCallback_;
} // namespace

extern "C" {
void ee_callCppInternal(const char* tag, const char* message) {
    callCppCallback_(tag, message);
}

void ee_runOnMainThreadCallback() {
    runOnMainThreadCallback_();
}

void ee_runOnMainThreadDelayedCallback(int key) {
    runOnMainThreadDelayedCallback_(key);
}
} // extern "C"

void ee_setCallCppCallback(const CallCppCallback& callback) {
    callCppCallback_ = callback;
}

void ee_setRunOnMainThreadCallback(const RunOnMainThreadCallback& callback) {
    runOnMainThreadCallback_ = callback;
}

void ee_setRunOnMainThreadDelayedCallback(
    const RunOnMainThreadDelayedCallback& callback) {
    runOnMainThreadDelayedCallback_ = callback;
}