//
//  ThreadUtils.cpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#include "ee/core/Thread.hpp"

#include <cassert>
#include <functional>
#include <queue>
#include <unordered_map>

#include <ee/cocos/CocosFwd.hpp>

#include "ee/core/SpinLock.hpp"
#include "ee/core/internal/ThreadImplCpp.hpp"
#include "ee/core/internal/ThreadImplJs.hpp"

#ifdef EE_X_ANDROID
#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniString.hpp"
#include "ee/core/internal/JniUtils.hpp"
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
using RunOnMainThreadCallback = std::function<void()>;
using RunOnMainThreadDelayedCallback = std::function<void(int key)>;

extern void
ee_setRunOnMainThreadCallback(const RunOnMainThreadCallback& callback);
extern void ee_setRunOnMainThreadDelayedCallback(
    const RunOnMainThreadDelayedCallback& callback);
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)

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

#ifdef EE_X_ANDROID
extern "C" {
JNIEXPORT void JNICALL
Java_com_ee_ThreadKt_ee_1runOnMainThreadCallback(JNIEnv* env, jclass clazz) {
    popInstantRunnable()();
}

JNIEXPORT void JNICALL Java_com_ee_ThreadKt_ee_1runOnMainThreadDelayedCallback(
    JNIEnv* env, jclass clazz, jint key) {
    popDelayedRunnable(key)();
}
} // extern "C"

bool ee_isMainThread() {
    return JniUtils::callStaticBooleanMethod( //
        "com/ee/ThreadKt",                    //
        "ee_isMainThread",                    //
        "()Z");
}

bool ee_runOnMainThread() {
    return JniUtils::callStaticBooleanMethod( //
        "com/ee/ThreadKt",                    //
        "ee_runOnMainThread",                 //
        "()Z");
}

void ee_runOnMainThreadDelayed(int key, float delay) {
    return JniUtils::callStaticVoidMethod( //
        "com/ee/ThreadKt",                 //
        "ee_runOnMainThreadDelayed",       //
        "(IF)V",                           //
        key,                               //
        delay);
}
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
bool ee_isMainThread();
bool ee_runOnMainThread();
void ee_runOnMainThreadDelayed(int key, float delay);
} // extern "C"
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)

using Self = Thread;

std::shared_ptr<IThreadImpl> Self::impl_;

void Self::initialize() {
#ifdef EE_X_COCOS_CPP
    impl_ = std::make_shared<ThreadImplCpp>();
#else  // EE_X_COCOS_CPP
    impl_ = std::make_shared<ThreadImplJs>();
#endif // EE_X_COCOS_CPP

#if defined(EE_X_IOS) || defined(EE_X_OSX)
    ee_setRunOnMainThreadCallback([] { //
        popInstantRunnable()();
    });
    ee_setRunOnMainThreadDelayedCallback([](int key) { //
        popDelayedRunnable(key)();
    });
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)
}

bool Self::isLibraryThread() {
    return impl_->isLibraryThread();
}

bool Self::runOnLibraryThread(const Runnable<>& runnable) {
    return impl_->runOnLibraryThread(runnable);
}

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
