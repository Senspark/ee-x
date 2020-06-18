//
//  ThreadUtils.cpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#include "ee/core/Thread.hpp"

#include <queue>
#include <unordered_map>

#include "ee/core/SpinLock.hpp"

#ifdef EE_X_ANDROID
#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniString.hpp"
#include "ee/core/internal/JniUtils.hpp"
#endif // EE_X_ANDROID

namespace ee {
namespace core {
namespace {
std::queue<Runnable<>> instantQueue_;
SpinLock instantLock_;

int queueCounter_;
std::unordered_map<int, Runnable<>> delayedQueue_;
SpinLock delayedLock_;

void pushInstantRunnable(const Runnable<>& runnable) {
    std::scoped_lock<SpinLock> lock(instantLock_);
    instantQueue_.push(runnable);
}

Runnable<> popInstantRunnable() {
    std::scoped_lock<SpinLock> lock(instantLock_);
    assert(not instantQueue_.empty());
    auto runnable = instantQueue_.front();
    instantQueue_.pop();
    return runnable;
}

int pushDelayedRunnable(const Runnable<>& runnable) {
    std::scoped_lock<SpinLock> lock(delayedLock_);
    delayedQueue_[++queueCounter_] = runnable;
    return queueCounter_;
}

Runnable<> popDelayedRunnable(int key) {
    std::scoped_lock<SpinLock> lock(delayedLock_);
    assert(not delayedQueue_.empty());
    auto iter = delayedQueue_.find(key);
    if (iter == delayedQueue_.cend()) {
        return nullptr;
    }
    auto runnable = iter->second;
    delayedQueue_.erase(iter);
    return runnable;
}
} // namespace

using Self = Thread;

#ifdef EE_X_ANDROID
extern "C" {
JNIEXPORT void JNICALL
Java_com_ee_core_internal_ThreadKt_ee_1runOnMainThreadCallback(JNIEnv* env,
                                                               jclass clazz) {
    popInstantRunnable()();
}

JNIEXPORT void JNICALL
Java_com_ee_core_internal_ThreadKt_ee_1runOnMainThreadDelayedCallback(
    JNIEnv* env, jclass clazz, jint key) {
    popDelayedRunnable(key)();
}
} // extern "C"

bool ee_isMainThread() {
    auto method = JniUtils::getStaticMethodInfo("com/ee/core/internal/ThreadKt",
                                                "ee_isMainThread", "()Z");
    return method->getEnv()->CallStaticBooleanMethod(method->getClass(),
                                                     method->getMethodId());
}

bool ee_runOnMainThread() {
    auto method = JniUtils::getStaticMethodInfo("com/ee/core/internal/ThreadKt",
                                                "ee_runOnMainThread", "()Z");
    return method->getEnv()->CallStaticBooleanMethod(method->getClass(),
                                                     method->getMethodId());
}

void ee_runOnMainThreadDelayed(int key, float delay) {
    auto method = JniUtils::getStaticMethodInfo(
        "com/ee/core/internal/ThreadKt", "ee_runOnMainThreadDelayed", "(IF)V");
    method->getEnv()->CallStaticVoidMethod(method->getClass(),
                                           method->getMethodId());
}
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
bool ee_isMainThread();
bool ee_runOnMainThread();
void ee_runOnMainThreadDelayed(int key, float delay);

void ee_runOnMainThreadCallback() {
    popInstantRunnable()();
}

void ee_runOnMainThreadDelayedCallback(int key) {
    popDelayedRunnable(key)();
}
} // extern "C"
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)

bool Self::isMainThread() {
    return ee_isMainThread();
}

bool Self::runOnMainThread(const Runnable<>& runnable) {
    if (isMainThread()) {
        runnable();
        return true;
    }
    pushInstantRunnable(runnable);
    return ee_runOnMainThread();
}

void Self::runOnMainThreadDelayed(const Runnable<>& runnable, float delay) {
    auto key = pushDelayedRunnable(runnable);
    ee_runOnMainThreadDelayed(key, delay);
}

void runOnUiThreadAndWait(const Runnable<>& runnable) {
    std::promise<void> promise;
    Thread::runOnMainThread([runnable, &promise] {
        runnable();
        promise.set_value();
    });
    return promise.get_future().get();
}
} // namespace core
} // namespace ee