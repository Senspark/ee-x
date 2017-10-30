//
//  Utils.hpp
//  ee_x
//
//  Created by Zinge on 10/9/17.
//
//

#ifndef EE_X_UTILS_HPP
#define EE_X_UTILS_HPP

#include <functional>
#include <future>
#include <string>

namespace ee {
namespace core {
/// Converts float to string without trailing zeroes.
std::string toString(float value);

/// Converts bool to string, used internally.
std::string toString(bool value);

/// Convert string to bool, used internally.
bool toBool(const std::string& value);

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

bool isTablet();

bool testConnection();
} // namespace core

using core::isMainThread;
using core::runOnUiThread;
using core::runOnUiThreadAndWait;
using core::runOnUiThreadAndWaitResult;
using core::getSHA1CertificateFingerprint;
using core::getVersionName;
using core::getVersionCode;
using core::isApplicationInstalled;
using core::openApplication;
using core::isTablet;
using core::testConnection;
} // namespace ee

#endif /* EE_X_UTILS_HPP */
