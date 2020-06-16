//
//  RandomUtils.cpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#include "ee/core/Random.hpp"

namespace ee {
namespace core {
using Self = Random;

std::mt19937& Self::getRandomEngine() {
    static std::random_device rd;
    static std::mt19937 engine(rd());
    return engine;
}
} // namespace core
} // namespace ee
