//
//  RandomUtils.hpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#ifndef EE_X_RANDOM_UTILS_HPP
#define EE_X_RANDOM_UTILS_HPP

#ifdef __cplusplus

#include <random>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
namespace detail {
std::mt19937& getRandomEngine();
} // namespace detail

/// Randomizes using C++11 engine.
template <class T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
T random(T min, T max) {
    if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(detail::getRandomEngine());
    } else {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(detail::getRandomEngine());
    }
}
} // namespace core

using core::random;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_RANDOM_UTILS_HPP */
