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
#include <sstream>
#include <thread>

#include "ee/Macro.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"

namespace ee {
namespace core {
/*
void runOnUiThreadAndWait(const Runnable<void>& runnable) {
#ifdef EE_X_ANDROID
    if (std::this_thread::get_id() == uiThreadId_) {
        runnable();
        return;
    }
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
*/

std::string toString(float value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string toString(bool value) {
    return value ? "true" : "false";
}

bool toBool(const std::string& value) {
    assert(value == "true" || value == "false");
    return value == "true";
}

namespace {
// clang-format off
constexpr auto k__isMainThread                  = "Utils_isMainThread";
constexpr auto k__runOnUiThread                 = "Utils_runOnUiThread";
constexpr auto k__runOnUiThreadCallback         = "Utils_runOnUiThreadCallback";
constexpr auto k__getSHA1CertificateFingerprint = "Utils_getSHA1CertificateFingerprint";
constexpr auto k__getVersionName                = "Utils_getVersionName";
constexpr auto k__getVersionCode                = "Utils_getVersionCode";
constexpr auto k__isApplicationInstalled        = "Utils_isApplicationInstalled";
constexpr auto k__openApplication               = "Utils_openApplication";
constexpr auto k__isTablet                      = "Utils_isTablet";
constexpr auto k__testConnection                = "Utils_testConnection";
// clang-format on
} // namespace

bool isMainThread() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__isMainThread);
    return toBool(response);
}

namespace {
std::queue<Runnable<void>> q_;
SpinLock lock_;

void pushRunnable(const Runnable<void>& runnable) {
    std::lock_guard<SpinLock> guard(lock_);
    q_.push(runnable);
}

Runnable<void> popRunnable() {
    std::lock_guard<SpinLock> guard(lock_);
    assert(not q_.empty());
    auto runnable = q_.front();
    q_.pop();
    return runnable;
}

void runOnUiThreadCallback() {
    popRunnable()();
}

void registerHandler() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        auto&& bridge = core::MessageBridge::getInstance();
        bridge.registerHandler(
            [](const std::string& message) {
                runOnUiThreadCallback();
                return "";
            },
            k__runOnUiThreadCallback);
    });
}
} // namespace

bool runOnUiThread(const Runnable<void>& runnable) {
    if (isMainThread()) {
        runnable();
        return true;
    }
    registerHandler();
    pushRunnable(runnable);
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__runOnUiThread);
    return toBool(response);
}

void runOnUiThreadAndWait(const Runnable<void>& runnable) {
    std::promise<void> promise;
    runOnUiThread([runnable, &promise] {
        runnable();
        promise.set_value();
    });
    return promise.get_future().get();
}

std::string getSHA1CertificateFingerprint() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getSHA1CertificateFingerprint);
}

std::string getVersionName() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getVersionName);
}

std::string getVersionCode() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getVersionCode);
}

bool isApplicationInstalled(const std::string& applicationId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__isApplicationInstalled);
    return toBool(response);
}

void openApplication(const std::string& applicationId) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__openApplication);
}

bool isTablet() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__isTablet);
    return toBool(response);
}

bool testConnection() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testConnection);
    return toBool(response);
}
} // namespace core
} // namespace ee
