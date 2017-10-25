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
void runOnUiThreadAndWait(const Runnable<void>& runnable) {
    std::promise<void> promise;
    runOnUiThread([runnable, &promise] {
        runnable();
        promise.set_value();
    });
    return promise.get_future().get();
}

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
constexpr auto k__getSHA1CertificateFingerprint = "Utils_getSHA1CertificateFingerprint";
constexpr auto k__getVersionName                = "Utils_getVersionName";
constexpr auto k__getVersionCode                = "Utils_getVersionCode";
constexpr auto k__testConnection                = "Utils_testConnection";
// clang-format on
} // namespace

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

bool testConnection(const std::string& hostName) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__testConnection);
    return toBool(response);
}
} // namespace core
} // namespace ee
