//
//  Utils.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_UTILS_HPP
#define EE_X_TEST_UTILS_HPP

#include <functional>

#include <ee/Cpp.hpp>

namespace eetest {
namespace detail {
using Tick = std::function<void()>;
} // namespace detail

void schedule(float delay, float interval, unsigned times,
              const detail::Tick& f);

void scheduleForever(float delay, float interval, const detail::Tick& f);

void scheduleOnce(float delay, const detail::Tick& f);

ee::Logger& getLogger();

std::string getCurrentThreadId();

void logCurrentThread();
} // namespace eetest

#endif /* EE_X_TEST_UTILS_HPP */
