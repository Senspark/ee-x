#include "ee/ads/internal/Retrier.hpp"

#include <ee/core/Delay.hpp>
#include <ee/core/Random.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace ads {
using Self = Retrier;

Self::Retrier(float base, float multiplier, float limit)
    : base_(base)
    , multiplier_(multiplier)
    , limit_(limit)
    , processing_(false)
    , stopRequested_(false) {}

Task<> Self::process(const Processor& processor) {
    if (processing_) {
        co_return;
    }
    processing_ = true;
    auto delay = base_;
    while (true) {
        auto x = random(0.0f, 1.0f);
        co_await Delay(delay + x);
        if (stopRequested_) {
            processing_ = false;
            stopRequested_ = false;
            co_return;
        }
        auto result = co_await processor();
        if (result) {
            processing_ = false;
            co_return;
        }
        delay *= multiplier_;
        delay = std::min(delay, limit_);
    }
}

void Self::stop() {
    if (processing_) {
        stopRequested_ = true;
    }
}
} // namespace ads
} // namespace ee
