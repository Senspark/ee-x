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
class RandomUtils {
public:
    static std::mt19937& getRandomEngine();

    /// Randomizes using C++11 engine.
    template <class T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
    static T random(T min, T max) {
        if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(getRandomEngine());
        } else {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(getRandomEngine());
        }
    }
};
} // namespace core

// Brings to ee::
constexpr auto random = [](auto&& min, auto&& max) {
    return core::RandomUtils::random(min, max);
};
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_RANDOM_UTILS_HPP */
