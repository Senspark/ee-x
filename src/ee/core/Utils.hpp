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
template <class T>
using Runnable = std::function<T()>;

/// Runs the specified runnable on the main thread.
void runOnUiThread(const Runnable<void>& runnable);

void runOnUiThreadAndWait(const Runnable<void>& runnable);

template <class T>
T runOnUiThreadAndWaitResult(const Runnable<T>& runnable) {
    std::promise<T> promise;
    runOnUiThread([runnable, &promise] { // Fix clang-format.
        promise.set_value(runnable());
    });
    return promise.get_future().get();
}

std::string toString(bool value);
bool toBool(const std::string& value);
} // namespace core

using core::runOnUiThread;
using core::runOnUiThreadAndWait;
using core::runOnUiThreadAndWaitResult;
} // namespace ee

#endif /* EE_X_UTILS_HPP */
