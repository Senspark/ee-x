//
//  ThreadUtils.hpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#ifndef EE_X_THREAD_HPP
#define EE_X_THREAD_HPP

#ifdef __cplusplus

#include <future>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
template <class T = void>
using Runnable = std::function<T()>;

class Thread {
public:
    /// Checks whether the current thread is the library thread (cocos2d...).
    static bool isLibraryThread();

    /// Runs the specified runnable on the library thread.
    /// @returns Whether the function is executed immediately.
    static bool runOnLibraryThread(const Runnable<>& runnable);

    /// Checks whether the current thread is the UI thread (Android) or Main
    /// thread (iOS).
    static bool isMainThread();

    /// Runs the specified runnable on the main thread.
    /// @returns Whether the function is executed immediately.
    static bool runOnMainThread(const Runnable<>& runnable);

    /// Runs the specifieid runnable on the main thread after a delay.
    static void runOnMainThreadDelayed(const Runnable<>& runnable, float delay);

private:
    friend PluginManager;

    static std::function<bool()> libraryThreadChecker_;
    static std::function<bool(const Runnable<>& runnable)>
        libraryThreadExecuter_;
};

/// Runs the specified runnable on the main thread and block the current
/// thread. If the current thread is the main thread, it will be executed
/// immediately.
[[deprecated]] void runOnUiThreadAndWait(const Runnable<>& runnable);

template <class T>
[[deprecated]] T runOnUiThreadAndWaitResult(const Runnable<T>& runnable) {
    std::promise<T> promise;
    runOnUiThread([runnable, &promise] { // Fix clang-format.
        promise.set_value(runnable());
    });
    return promise.get_future().get();
}
} // namespace core

constexpr auto isMainThread = &Thread::isMainThread;
constexpr auto runOnMainThread = &Thread::runOnMainThread;
constexpr auto runOnMainThreadDelayed = &Thread::runOnMainThreadDelayed;
[[deprecated]] constexpr auto runOnUiThread = runOnMainThread;
[[deprecated]] constexpr auto runOnUiThreadDelayed = runOnMainThreadDelayed;
using core::runOnUiThreadAndWait;
using core::runOnUiThreadAndWaitResult;
} // namespace ee

#endif // __cplusplus

#if defined(EE_X_IOS) || defined(EE_X_OSX)
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern void ee_runOnMainThreadCallback();
extern void ee_runOnMainThreadDelayedCallback(int key);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)

#endif /* EE_X_THREAD_HPP */
