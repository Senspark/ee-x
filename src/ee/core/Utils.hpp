//
//  Utils.hpp
//  ee_x
//
//  Created by Zinge on 10/9/17.
//
//

#ifndef EE_X_UTILS_HPP
#define EE_X_UTILS_HPP

#include <cstdarg>
#include <functional>
#include <future>
#include <string>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {

// lower string
std::string str_tolower(std::string s);

/// Converts float to string without trailing zeroes.
std::string toString(float value);

/// Converts bool to string, used internally.
std::string toString(bool value);

/// Convert string to bool, used internally.
bool toBool(const std::string& value);

/// http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
std::string format(std::string formatString, ...);
std::string format(std::string formatString, std::va_list args);

void log(const LogLevel& level, const std::string& tag,
         const std::string& message);

/// Checks whether the current thread is the main thread (UI thread on Android).
bool isMainThread();

template <class T>
using Runnable = std::function<T()>;

/// Runs the specified runnable on the main thread.
bool runOnUiThread(const Runnable<void>& runnable);

/// Runs the specified runnable on the main thread and block the current thread.
/// If the current thread is the main thread, it will be executed immediately.
void runOnUiThreadAndWait(const Runnable<void>& runnable);

template <class T>
T runOnUiThreadAndWaitResult(const Runnable<T>& runnable) {
    std::promise<T> promise;
    runOnUiThread([runnable, &promise] { // Fix clang-format.
        promise.set_value(runnable());
    });
    return promise.get_future().get();
}

/// Android only.
/// iOS returns an empty string.
std::string getSHA1CertificateFingerprint();

/// Gets the version name of the application.
std::string getVersionName();

/// Gets the version code of the application.
std::string getVersionCode();

/// Checks whether an application with the specified package name (Android) or
/// scheme (iOS) is installed.
bool isApplicationInstalled(const std::string& applicationId);

/// Opens an application with the specified package name (Android) or scheme
/// (iOS).
bool openApplication(const std::string& applicationId);

bool sendMail(const std::string& recipient, const std::string& subject,
              const std::string& body);

bool isTablet();

bool testConnection();

/// Gets device's unique ID.
void getDeviceId(const std::function<void(const std::string&)>& callback);

std::string dumpBacktrace(size_t count);

void runOnUiThreadDelayed(const std::function<void()>& func, float delay);

bool isInstantApp();

void showInstallPrompt(const std::string& url, const std::string& referrer);

/// Gets the name of the application.
std::string getApplicationName();

/// https://developer.android.com/reference/android/view/DisplayCutout
struct SafeInset {
    /// The inset from the left which avoids the display cutout in pixels.
    int left;

    /// The inset from the right which avoids the display cutout in pixels.
    int right;

    /// The inset from the top which avoids the display cutout in pixels.
    int top;

    /// The inset from the bottom which avoids the display cutout in pixels.
    int bottom;
};

SafeInset getSafeInset();

/// Gets the screen density, i.e. pixel to dp ratio.
float getDensity();
} // namespace core

using core::format;
using core::getApplicationName;
using core::getDensity;
using core::getDeviceId;
using core::getSafeInset;
using core::getSHA1CertificateFingerprint;
using core::getVersionCode;
using core::getVersionName;
using core::isApplicationInstalled;
using core::isMainThread;
using core::isTablet;
using core::log;
using core::openApplication;
using core::runOnUiThread;
using core::runOnUiThreadAndWait;
using core::runOnUiThreadAndWaitResult;
using core::sendMail;
using core::testConnection;

using core::SafeInset;
} // namespace ee

#endif /* EE_X_UTILS_HPP */
