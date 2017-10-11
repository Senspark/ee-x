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

#include "ee/Macro.hpp"
#include "ee/core/Utils.hpp"

#ifdef EE_X_ANDROID
#include "ee/core/internal/JniMethodInfo.hpp"
#include <ee/core/JniUtils.hpp>
#include <ee/core/SpinLock.hpp>
#endif // EE_X_ANDROID

namespace ee {
namespace core {
#ifdef EE_X_ANDROID
namespace {
std::queue<Runnable> q_;
SpinLock lock_;
} // namespace

extern "C" {
JNIEXPORT void JNICALL Java_com_ee_core_internal_Utils_signal(JNIEnv* env,
                                                              jclass clazz) {
    std::lock_guard<SpinLock> guard(lock_);
    assert(not q_.empty());
    q_.front()();
    q_.pop();
}
} // extern "C"
#endif // EE_X_ANDROID

void runOnUiThread(const Runnable& runnable) {
#ifdef EE_X_ANDROID
    std::lock_guard<SpinLock> guard(lock_);
    q_.push(runnable);
    auto methodInfo = JniUtils::getStaticMethodInfo(
        "com/ee/core/internal/Utils", "runOnUiThread", "()V");
    if (methodInfo == nullptr) {
        throw std::runtime_error("Method not found!");
    }
    methodInfo->getEnv()->CallStaticVoidMethod(methodInfo->getClass(),
                                               methodInfo->getMethodId());
#else  // EE_X_ANDROID
    runnable();
#endif // EE_X_ANDROID
}

void runOnUiThreadAndWait(const Runnable& runnable) {
#ifdef EE_X_ANDROID
    std::mutex mtx;
    std::condition_variable cv;
    bool processed = false;
    runOnUiThread([runnable, &mtx, &cv, &processed] {
        std::unique_lock<std::mutex> lock(mtx);
        runnable();
        processed = true;
        lock.unlock();
        cv.notify_one();
    });
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&processed] { return processed; });
#else  // EE_X_ANDROID
    runnable();
#endif // EE_X_ANDROID
}
} // namespace core
} // namespace ee
