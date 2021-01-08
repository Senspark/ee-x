//
//  SwiftBridge.cpp
//  ee-x
//
//  Created by eps on 7/30/20.
//

#include <string>

using CallCppPointer = void (*)(const char*, const char*);
using RunOnMainThreadCallbackPointer = void (*)();
using RunOnMainThreadDelayedCallbackPointer = void (*)(int);

namespace {
CallCppPointer callCppPointer_;
RunOnMainThreadCallbackPointer runOnMainThreadCallbackPointer_;
RunOnMainThreadDelayedCallbackPointer runOnMainThreadDelayedCallbackPointer_;
} // namespace

extern "C" {
void ee_initializeMessageBridge(CallCppPointer pointer) {
    callCppPointer_ = pointer;
}

void ee_callCppInternal(const char* tag, const char* message) {
    callCppPointer_(tag, message);
}

void ee_initializeThread(RunOnMainThreadCallbackPointer pointer1,
                         RunOnMainThreadDelayedCallbackPointer pointer2) {
    runOnMainThreadCallbackPointer_ = pointer1;
    runOnMainThreadDelayedCallbackPointer_ = pointer2;
}

void ee_runOnMainThreadCallback() {
    runOnMainThreadCallbackPointer_();
}

void ee_runOnMainThreadDelayedCallback(int key) {
    runOnMainThreadDelayedCallbackPointer_(key);
}
} // extern "C"