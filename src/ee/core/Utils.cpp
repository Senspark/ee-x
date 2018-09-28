//
//  Utils.cpp
//  ee_x
//
//  Created by Zinge on 10/9/17.
//
//

#include "ee/core/Utils.hpp"

#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <dlfcn.h>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <unwind.h>

#include <ee/nlohmann/json.hpp>

#include "ee/core/MessageBridge.hpp"
#include "ee/core/internal/SpinLock.hpp"

namespace ee {
namespace core {

struct BacktraceState {
    void** current;
    void** end;
};
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

std::string str_tolower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); } // correct
    );
    return s;
}

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

std::string format(std::string formatString, ...) {
    std::va_list args;
    va_start(args, formatString);
    auto result = format(formatString, args);
    va_end(args);
    return result;
}

std::string format(std::string formatString, std::va_list args) {
    std::size_t n = formatString.size() * 2;
    while (true) {
        auto formatted = std::make_unique<char[]>(n);
        std::va_list temp_args;
        va_copy(temp_args, args);
        auto final_n =
            std::vsnprintf(&formatted[0], n, formatString.c_str(), temp_args);
        va_end(temp_args);
        if (final_n < 0 || final_n >= static_cast<int>(n)) {
            n += static_cast<std::size_t>(
                std::abs(final_n - static_cast<int>(n) + 1));
        } else {
            return std::string(formatted.get());
        }
    }
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
constexpr auto k__sendMail                      = "Utils_sendMail";
constexpr auto k__isTablet                      = "Utils_isTablet";
constexpr auto k__testConnection                = "Utils_testConnection";
constexpr auto k__getDeviceId                   = "Utils_getDeviceId";
constexpr auto k__runFunctionDelay              = "Utils_funFunctionDelay";
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
        auto&& bridge = MessageBridge::getInstance();
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
    auto&& bridge = MessageBridge::getInstance();
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
    auto response = bridge.call(k__isApplicationInstalled, applicationId);
    return toBool(response);
}

bool openApplication(const std::string& applicationId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__openApplication, applicationId);
    return toBool(response);
}

bool sendMail(const std::string& recipient, const std::string& subject,
              const std::string& body) {
    nlohmann::json json;
    json["recipient"] = recipient;
    json["subject"] = subject;
    json["body"] = body;
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__sendMail, json.dump());
    return toBool(response);
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

void getDeviceId(const std::function<void(const std::string&)>& callback) {
    auto&& bridge = MessageBridge::getInstance();
    static int callbackCounter = 0;
    auto callbackTag = ee::format("getDeviceId_%d", callbackCounter++);
    bridge.registerHandler(
        [callback, callbackTag, &bridge](const std::string& msg) {
            bridge.deregisterHandler(callbackTag);
            if (callback) {
                callback(msg);
            }
            return "";
        },
        callbackTag);
    nlohmann::json json;
    json["callback_id"] = callbackTag;
    bridge.call(k__getDeviceId, json.dump());
}

void runFunctionDelay(const std::function<void()>& func, float delay) {
    auto&& bridge = MessageBridge::getInstance();
    static int funcCallbackCounter = 0;
    auto callbackTag = ee::format("runFunctionDelay_%d", funcCallbackCounter);
    bridge.registerHandler(
        [func, callbackTag, &bridge](const std::string& msg) {
            bridge.deregisterHandler(callbackTag);
            if (func) {
                func();
            }
            return "";
        },
        callbackTag);
    nlohmann::json json;
    json["callback_id"] = callbackTag;
    json["delay_time"] = delay;
    bridge.call(k__runFunctionDelay, json.dump());
}

_Unwind_Reason_Code unwindCallback(struct _Unwind_Context* context, void* arg) {
    BacktraceState* state = static_cast<BacktraceState*>(arg);
    uintptr_t pc = _Unwind_GetIP(context);
    if (pc) {
        if (state->current == state->end) {
            return _URC_END_OF_STACK;
        } else {
            *state->current++ = reinterpret_cast<void*>(pc);
        }
    }
    return _URC_NO_REASON;
}

size_t captureBacktrace(void** buffer, size_t max) {
    BacktraceState state = {buffer, buffer + max};
    _Unwind_Backtrace(unwindCallback, &state);

    return state.current - buffer;
}

void dumpBacktrace(std::ostream& os, void** buffer, size_t count) {
    for (size_t idx = 0; idx < count; ++idx) {
        const void* addr = buffer[idx];
        const char* symbol = "";

        Dl_info info;
        if (dladdr(addr, &info) && info.dli_sname) {
            symbol = info.dli_sname;
        }

        os << "  #" << std::setw(2) << idx << ": " << addr << "  " << symbol
           << "\n";
    }
}

std::string dumpBacktrace(size_t count) {
    void* buffer[count];
    std::ostringstream oss;

    dumpBacktrace(oss, buffer, captureBacktrace(buffer, count));
    return oss.str();
}

} // namespace core
} // namespace ee
