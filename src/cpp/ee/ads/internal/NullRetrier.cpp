//
//  NullRetrier.cpp
//  Pods
//
//  Created by eps on 6/2/21.
//

#include "ee/ads/internal/NullRetrier.hpp"

#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = NullRetrier;

Task<> Self::process(const Processor& processor) {
    co_return;
}

void Self::stop() {}
} // namespace ads
} // namespace ee
