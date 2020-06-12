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

#ifdef EE_X_ANDROID
#include <android/log.h>
#endif // EE_X_ANDROID

#include <ee/nlohmann/json.hpp>

#include "ee/core/LambdaAwaiter.hpp"
#include "ee/core/LogLevel.hpp"
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
const std::string kPrefix = "Utils_";

const auto kIsMainThread             = kPrefix + "isMainThread";
const auto kRunOnUiThread            = kPrefix + "runOnUiThread";
const auto kRunOnUiThreadDelayed     = kPrefix + "runOnUiThreadDelayed";
const auto kRunOnUiThreadCallback    = kPrefix + "runOnUiThreadCallback";

const auto kIsApplicationInstalled   = kPrefix + "isApplicationInstalled";
const auto kOpenApplication          = kPrefix + "openApplication";

const auto kGetApplicationId         = kPrefix + "getApplicationId";
const auto kGetApplicationName       = kPrefix + "getApplicationName";
const auto kGetVersionName           = kPrefix + "getVersionName";
const auto kGetVersionCode           = kPrefix + "getVersionCode";

const auto kGetApplicationSignatures = kPrefix + "getApplicationSignatures";
const auto kIsInstantApp             = kPrefix + "isInstantApp";
const auto kIsTablet                 = kPrefix + "isTablet";
const auto kGetDensity               = kPrefix + "getDensity";
const auto kGetDeviceId              = kPrefix + "getDeviceId";
const auto kGetSafeInset             = kPrefix + "getSafeInset";

const auto kSendMail                 = kPrefix + "sendMail";
const auto kTestConnection           = kPrefix + "testConnection";
const auto kShowInstallPrompt        = kPrefix + "showInstallPrompt";
// clang-format on
} // namespace

bool isMainThread() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kIsMainThread);
    return toBool(response);
}

namespace {
std::queue<Runnable<>> q_;
SpinLock lock_;

void pushRunnable(const Runnable<>& runnable) {
    std::lock_guard<SpinLock> guard(lock_);
    q_.push(runnable);
}

Runnable<> popRunnable() {
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
            kRunOnUiThreadCallback);
    });
}
} // namespace

bool runOnUiThread(const Runnable<>& runnable) {
    if (isMainThread()) {
        runnable();
        return true;
    }
    registerHandler();
    pushRunnable(runnable);
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kRunOnUiThread);
    return toBool(response);
}

void runOnUiThreadDelayed(const Runnable<>& runnable, float delay) {
    noAwait([runnable, delay]() -> Task<> {
        nlohmann::json json;
        json["delay"] = delay;
        auto&& bridge = MessageBridge::getInstance();
        auto response =
            co_await bridge.callAsync(kRunOnUiThreadDelayed, json.dump());
        if (runnable) {
            runnable();
        }
    });
}

void runOnUiThreadAndWait(const Runnable<>& runnable) {
    std::promise<void> promise;
    runOnUiThread([runnable, &promise] {
        runnable();
        promise.set_value();
    });
    return promise.get_future().get();
}

bool isApplicationInstalled(const std::string& applicationId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kIsApplicationInstalled, applicationId);
    return toBool(response);
}

bool openApplication(const std::string& applicationId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kOpenApplication, applicationId);
    return toBool(response);
}

std::string getApplicationId() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(kGetApplicationId);
}

std::string getApplicationName() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(kGetApplicationName);
}

std::string getVersionName() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(kGetVersionName);
}

std::string getVersionCode() {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(kGetVersionCode);
}

std::string getSHA1CertificateFingerprint() {
#ifdef EE_X_ANDROID
    auto&& bridge = MessageBridge::getInstance();
    nlohmann::json request;
    request["algorithm"] = "SHA1";
    auto response = bridge.call(kGetApplicationSignatures, request.dump());
    auto json = nlohmann::json::parse(response);
    auto signatures = json["signatures"];
    return signatures.empty() ? "" : signatures[0];
#else  // EE_X_ANDROID
    return "";
#endif // EE_X_ANDROID
}

bool isInstantApp() {
#ifdef EE_X_ANDROID
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kIsInstantApp);
    return toBool(response);
#else  // EE_X_ANDROID
    return false;
#endif // EE_X_ANDROID
}

bool isTablet() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kIsTablet);
    return toBool(response);
}

float getDensity() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kGetDensity);
    return std::stof(response);
}

Task<std::string> getDeviceId() {
    auto&& bridge = MessageBridge::getInstance();
    auto response = co_await bridge.callAsync(kGetDeviceId);
    co_return response;
}

SafeInset getSafeInset() {
    SafeInset result;
#ifdef EE_X_ANDROID
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(kGetSafeInset);
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
    auto response = bridge.call(kSendMail, json.dump());
    return toBool(response);
}

Task<bool> testConnection(const std::string& hostName, float timeOut) {
    nlohmann::json json;
    json["host_name"] = hostName;
    json["time_out"] = timeOut;
    auto&& bridge = MessageBridge::getInstance();
    auto response = co_await bridge.callAsync(kTestConnection, json.dump());
    co_return toBool(response);
}

void showInstallPrompt(const std::string& url, const std::string& referrer) {
#ifdef EE_X_ANDROID
    nlohmann::json json;
    json["url"] = url;
    json["referrer"] = referrer;
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(kShowInstallPrompt, json.dump());
#endif // EE_X_ANDROID
}

#ifdef EE_X_ANDROID
void log(const LogLevel& level, const std::string& tag,
         const std::string& message) {
    __android_log_print(level.priority, tag.c_str(), "%s", message.c_str());
}
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
void ee_staticLog(const char* message);
} // extern "C"

void log(const LogLevel& level, const std::string& tag,
         const std::string& message) {
    std::string buffer;
    buffer.reserve(level.desc.size() + 1 + tag.size() + 1 + message.size());
    buffer += level.desc;
    buffer += " ";
    buffer += tag;
    buffer += " ";
    buffer += message;
    ee_staticLog(buffer.c_str());
}
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)
} // namespace core
} // namespace ee
