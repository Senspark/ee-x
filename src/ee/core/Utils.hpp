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

std::string toString(bool value);
bool toBool(const std::string& value);

bool isMainThread();

template <class T>
using Runnable = std::function<T()>;

/// Runs the specified runnable on the main thread.
bool runOnUiThread(const Runnable<void>& runnable);

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

bool isApplicationInstalled(const std::string& applicationId);

void openApplication(const std::string& applicationId);

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
using core::isTablet;
using core::testConnection;
} // namespace ee

#endif /* EE_X_UTILS_HPP */
