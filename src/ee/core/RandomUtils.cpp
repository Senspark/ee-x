//
//  RandomUtils.cpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#include "ee/core/RandomUtils.hpp"

namespace ee {
namespace core {
namespace detail {
std::mt19937& getRandomEngine() {
    static std::random_device rd;
    static std::mt19937 engine(rd());
    return engine;
}
} // namespace detail
} // namespace core
} // namespace ee
