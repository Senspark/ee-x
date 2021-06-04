//
//  LockCapper.cpp
//  Pods
//
//  Created by eps on 6/2/21.
//

#include "ee/ads/internal/LockCapper.hpp"

namespace ee {
namespace ads {
using Self = LockCapper;

Self::LockCapper()
    : locked_(false) {}

Self::~LockCapper() = default;

bool Self::isCapped() const {
    return locked_;
}

void Self::cap() {}

void Self::lock() {
    locked_ = true;
}

void Self::unlock() {
    locked_ = false;
}
} // namespace ads
} // namespace ee
