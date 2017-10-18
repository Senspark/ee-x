//
//  Utils.cpp
//  ee_x
//
//  Created by Zinge on 10/9/17.
//
//

#include <cassert>
#include <mutex>
#include <queue>
#include <thread>

#include "ee/Macro.hpp"
#include "ee/core/JniUtils.hpp"
#include "ee/core/SpinLock.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/JniMethodInfo.hpp"

namespace ee {
namespace core {
namespace {
std::queue<Runnable<void>> q_;
SpinLock lock_;
std::thread::id uiThreadId_;

extern "C" {
JNIEXPORT void JNICALL Java_com_ee_core_internal_Utils_signal(JNIEnv* env,
                                                              jclass clazz) {
    uiThreadId_ = std::this_thread::get_id();
    std::lock_guard<SpinLock> guard(lock_);
    assert(not q_.empty());
    q_.front()();
    q_.pop();
}
} // extern "C"
} // namespace

void runOnUiThread(const Runnable<void>& runnable) {
    if (std::this_thread::get_id() == uiThreadId_) {
        runnable();
        return;
    }
    std::lock_guard<SpinLock> guard(lock_);
    q_.push(runnable);
    auto methodInfo = JniUtils::getStaticMethodInfo(
        "com/ee/core/internal/Utils", "runOnUiThread", "()V");
    if (methodInfo == nullptr) {
        throw std::runtime_error("Method not found!");
    }
    methodInfo->getEnv()->CallStaticVoidMethod(methodInfo->getClass(),
                                               methodInfo->getMethodId());
}
} // namespace core
} // namespace ee
