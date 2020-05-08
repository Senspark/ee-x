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
#include <random>
#include <sstream>
#include <thread>
#include <unwind.h>

#include <ee/nlohmann/json.hpp>

#include "ee/core/LambdaAwaiter.hpp"
#include "ee/core/SpinLock.hpp"
#include "ee/core/internal/MessageBridge.hpp"

namespace ee {
namespace core {
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

namespace detail {
std::mt19937& getRandomEngine() {
    static std::random_device rd;
    static std::mt19937 engine(rd());
    return engine;
}
} // namespace detail

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

struct BacktraceState {
    void** current;
    void** end;
};

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

namespace {
// clang-format off
constexpr auto k__isMainThread                  = "Utils_isMainThread";
constexpr auto k__runOnUiThread                 = "Utils_runOnUiThread";
constexpr auto k__runOnUiThreadDelayed          = "Utils_runOnUiThreadDelayed";
constexpr auto k__runOnUiThreadCallback         = "Utils_runOnUiThreadCallback";

constexpr auto k__isApplicationInstalled        = "Utils_isApplicationInstalled";
constexpr auto k__openApplication               = "Utils_openApplication";

constexpr auto k__getApplicationId              = "Utils_getApplicationId";
constexpr auto k__getApplicationName            = "Utils_getApplicationName";
constexpr auto k__getVersionName                = "Utils_getVersionName";
constexpr auto k__getVersionCode                = "Utils_getVersionCode";

constexpr auto k__getSHA1CertificateFingerprint = "Utils_getSHA1CertificateFingerprint";
constexpr auto k__isInstantApp                  = "Utils_isInstantApp";
constexpr auto k__isTablet                      = "Utils_isTablet";
constexpr auto k__getDensity                    = "Utils_getDensity";
constexpr auto k__getDeviceId                   = "Utils_getDeviceId";
constexpr auto k__getSafeInset                  = "Utils_getSafeInset";

constexpr auto k__sendMail                      = "Utils_sendMail";
constexpr auto k__testConnection                = "Utils_testConnection";
constexpr auto k__showInstallPrompt             = "Utils_showInstallPrompt";
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

void runOnUiThreadDelayed(const std::function<void()>& func, float delay) {
    auto&& bridge = MessageBridge::getInstance();
    static int counter = 0;
    auto callbackTag = ee::format("runOnUiThreadDelayed_%d", counter++);
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
    bridge.call(k__runOnUiThreadDelayed, json.dump());
}

void runOnUiThreadAndWait(const Runnable<void>& runnable) {
    std::promise<void> promise;
    runOnUiThread([runnable, &promise] {
        runnable();
        promise.set_value();
    });
    return promise.get_future().get();
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

std::string getApplicationId() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getApplicationId);
}

std::string getApplicationName() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getApplicationName);
}

std::string getVersionName() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getVersionName);
}

std::string getVersionCode() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getVersionCode);
}

std::string getSHA1CertificateFingerprint() {
#ifdef EE_X_ANDROID
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getSHA1CertificateFingerprint);
#else  // EE_X_ANDROID
    return "";
#endif // EE_X_ANDROID
}

bool isInstantApp() {
#ifdef EE_X_ANDROID
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__isInstantApp);
    return toBool(response);
#else  // EE_X_ANDROID
    return false;
#endif // EE_X_ANDROID
}

bool isTablet() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__isTablet);
    return toBool(response);
}

float getDensity() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__getDensity);
    return std::stof(response);
}

Task<std::string> getDeviceId() {
    auto awaiter = LambdaAwaiter<std::string>([](auto&& resolve) {
        static int counter;
        auto callbackTag = "getDeviceId_" + std::to_string(counter++);

        auto&& bridge = MessageBridge::getInstance();
        bridge.registerHandler(
            [resolve, &bridge, callbackTag](const std::string& message) {
                bridge.deregisterHandler(callbackTag);
                resolve(message);
                return "";
            },
            callbackTag);

        nlohmann::json json;
        json["callback_tag"] = callbackTag;
        bridge.call(k__getDeviceId, json.dump());
    });
    co_return co_await awaiter;
}

SafeInset getSafeInset() {
    SafeInset result;
#ifdef EE_X_ANDROID
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__getSafeInset);
    auto json = nlohmann::json::parse(response);

    result.left = json["left"];
    result.right = json["right"];
    result.top = json["top"];
    result.bottom = json["bottom"];
#else  // EE_X_ANDROID
    // TODO.
    result.left = 0;
    result.right = 0;
    result.top = 0;
    result.bottom = 0;
#endif // EE_X_ANDROID
    return result;
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

Task<bool> testConnection(const std::string& hostName, float timeOut) {
    auto awaiter = LambdaAwaiter<bool>([hostName, timeOut](auto&& resolve) {
        static int counter;
        auto callbackTag = k__testConnection + std::to_string(counter++);

        auto&& bridge = MessageBridge::getInstance();
        bridge.registerHandler(
            [resolve, &bridge, callbackTag](const std::string& message) {
                bridge.deregisterHandler(callbackTag);
                auto result = toBool(message);
                resolve(result);
                return "";
            },
            callbackTag);

        nlohmann::json json;
        json["callback_tag"] = callbackTag;
        json["host_name"] = hostName;
        json["time_out"] = timeOut;
        bridge.call(k__testConnection, json.dump());
    });
    co_return co_await awaiter;
}

void showInstallPrompt(const std::string& url, const std::string& referrer) {
#ifdef EE_X_ANDROID
    nlohmann::json json;
    json["url"] = url;
    json["referrer"] = referrer;
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__showInstallPrompt, json.dump());
#endif // EE_X_ANDROID
}
} // namespace core
} // namespace ee
