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

namespace ee {
namespace core {
using Runnable = std::function<void()>;

void runOnUiThread(const Runnable& runnable);
} // namespace core

using core::runOnUiThread;
} // namespace ee

#endif /* EE_X_UTILS_HPP */
