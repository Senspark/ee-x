//
//  Capper.cpp
//  Pods
//
//  Created by eps on 2/22/21.
//

#include "ee/ads/internal/Capper.hpp"

#include <ee/core/Delay.hpp>
#include <ee/core/NoAwait.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = Capper;

Self::Capper(float interval)
    : interval_(interval)
    , capped_(false)
    , locked_(false) {}

Self::~Capper() = default;

bool Self::isCapped() const {
    return capped_ || locked_;
}

void Self::cap() {
    capped_ = true;
    noAwait([this]() -> Task<> {
        co_await Delay(interval_);
        capped_ = false;
    });
}

void Self::lock() {
    locked_ = true;
}

void Self::unlock() {
    locked_ = false;
}
} // namespace ads
} // namespace ee
