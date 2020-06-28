//
//  RandomUtils.hpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#ifndef EE_X_RANDOM_HPP
#define EE_X_RANDOM_HPP

#ifdef __cplusplus

#include <random>

namespace ee {
namespace core {
class Random {
public:
    static std::mt19937& getRandomEngine();

    /// Randomizes using C++11 engine.
    template <class T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
    static T range(T min, T max) {
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
    return core::Random::range(min, max);
};
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_RANDOM_HPP */
