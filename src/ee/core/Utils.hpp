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
#include <memory>
#include <string>

namespace ee {
namespace core {
using Runnable = std::function<void()>;

/// Runs the specified runnable on the main thread.
void runOnUiThread(const Runnable& runnable);

/// Runs the specified runnable on the main thread and wait for it to finish.
void runOnUiThreadAndWait(const Runnable& runnable);

template <class T>
T runOnUiThreadAndWaitResult(const std::function<T()>& runnable) {
    std::unique_ptr<T> result;
    runOnUiThreadAndWait(
        [runnable, result] { result.reset(new T(runnable())); });
    return *result;
}

std::string toString(bool value);
bool toBool(const std::string& value);
} // namespace core

using core::runOnUiThread;
using core::runOnUiThreadAndWait;
} // namespace ee

#endif /* EE_X_UTILS_HPP */
