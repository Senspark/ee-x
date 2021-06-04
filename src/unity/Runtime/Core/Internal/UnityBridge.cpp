//
//  SwiftBridge.cpp
//  ee-x
//
//  Created by eps on 7/30/20.
//

#include <functional>
#include <string>

using CallCppPointer = void (*)(const char*, const char*);
using RunOnMainThreadCallbackPointer = void (*)();
using RunOnMainThreadDelayedCallbackPointer = void (*)(int);

using CallCppCallback =
    std::function<void(const std::string& tag, const std::string& message)>;
using RunOnMainThreadCallback = std::function<void()>;
using RunOnMainThreadDelayedCallback = std::function<void(int key)>;

extern void ee_setCallCppCallback(const CallCppCallback& callback);
extern void
ee_setRunOnMainThreadCallback(const RunOnMainThreadCallback& callback);
extern void ee_setRunOnMainThreadDelayedCallback(
    const RunOnMainThreadDelayedCallback& callback);

namespace {
CallCppPointer callCppPointer_;
RunOnMainThreadCallbackPointer runOnMainThreadCallbackPointer_;
RunOnMainThreadDelayedCallbackPointer runOnMainThreadDelayedCallbackPointer_;
} // namespace

extern "C" {
void ee_initializeMessageBridge(CallCppPointer pointer) {
    callCppPointer_ = pointer;
    ee_setCallCppCallback(
        [](const std::string& tag, const std::string& message) { //
            callCppPointer_(tag.c_str(), message.c_str());
        });
}

void ee_initializeThread(RunOnMainThreadCallbackPointer pointer1,
                         RunOnMainThreadDelayedCallbackPointer pointer2) {
    runOnMainThreadCallbackPointer_ = pointer1;
    runOnMainThreadDelayedCallbackPointer_ = pointer2;
    ee_setRunOnMainThreadCallback([] { //
        runOnMainThreadCallbackPointer_();
    });
    ee_setRunOnMainThreadDelayedCallback([](int key) { //
        runOnMainThreadDelayedCallbackPointer_(key);
    });
}
} // extern "C"