//
//  GuardedAd.cpp
//  Pods
//
//  Created by eps on 6/2/21.
//

#include "ee/ads/internal/GuardedAd.hpp"

#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

#include "ee/ads/internal/ICapper.hpp"
#include "ee/ads/internal/IRetrier.hpp"

namespace ee {
namespace ads {
using Self = GuardedAd;

Self::GuardedAd(const std::shared_ptr<IAd>& ad,
                const std::shared_ptr<ICapper>& capper,
                const std::shared_ptr<IRetrier>& retrier)
    : ad_(ad)
    , capper_(capper)
    , retrier_(retrier) {
    handle_ = std::make_unique<ObserverHandle>();
    handle_->bind(*ad_).addObserver({
        .onLoaded =
            [this] {
                // Propagation.
                dispatchEvent([](auto&& observer) {
                    if (observer.onLoaded) {
                        observer.onLoaded();
                    }
                });
            },
        .onLoadResult =
            [this](const AdLoadResult& result) {
                // Propagation.
                dispatchEvent([result](auto&& observer) {
                    if (observer.onLoadResult) {
                        observer.onLoadResult(result);
                    }
                });
            },
        .onClicked =
            [this] {
                // Propagation.
                dispatchEvent([](auto&& observer) {
                    if (observer.onClicked) {
                        observer.onClicked();
                    }
                });
            },
    });
    loading_ = false;
    loaded_ = false;
}

Self::~GuardedAd() = default;

void Self::destroy() {
    ad_->destroy();
    handle_->clear();
    retrier_->stop();
}

bool Self::isLoaded() const {
    return loaded_;
}

bool Self::isLoading() const {
    return loading_;
}

Task<bool> Self::load() {
    if (isLoaded()) {
        co_return true;
    }
    if (isDisplaying()) {
        co_return false;
    }
    if (isLoading()) {
        co_return false;
    }
    loading_ = true;
    loaded_ = co_await loadInternal();
    if (isLoaded()) {
        retrier_->stop();
    } else {
        noAwait([this]() -> Task<> {
            co_await retrier_->process([this]() -> Task<bool> {
                co_return loaded_ = co_await loadInternal();
            });
        });
    }
    loading_ = false;
    co_return loaded_;
}

Task<bool> Self::loadInternal() {
    if (capper_->isCapped()) {
        co_return false;
    }
    capper_->cap();
    co_return co_await ad_->load();
}
} // namespace ads
} // namespace ee
