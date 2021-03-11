//
//  LazyFullScreenAd.cpp
//  Pods
//
//  Created by eps on 2/22/21.
//

#include "ee/services/internal/LazyFullScreenAd.hpp"

#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/WhenAll.hpp>

#include "ee/ads/internal/ICapper.hpp"

namespace ee {
namespace services {
using Self = LazyFullScreenAd;

Self::LazyFullScreenAd(const std::shared_ptr<ads::ICapper>& displayCapper)
    : displayCapper_(displayCapper) {
    handle_ = std::make_unique<ObserverHandle>();
}

Self::~LazyFullScreenAd() = default;

void Self::setAd(const std::shared_ptr<IFullScreenAd>& ad) {
    handle_->clear();
    if (ad == nullptr) {
        return;
    }
    (*handle_) //
        .bind(*ad)
        .addObserver({
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
    ad_ = ad;
    noAwait(ad_->load());
}

void Self::destroy() {
    if (ad_ != nullptr) {
        ad_->destroy();
    }
    handle_->clear();
}

bool Self::isLoaded() const {
    if (ad_ == nullptr) {
        return false;
    }
    return ad_->isLoaded();
}

Task<bool> Self::load() {
    if (ad_ == nullptr) {
        co_return false;
    }
    auto result = co_await ad_->load();
    co_return result;
}

Task<AdResult> Self::show() {
    if (ad_ == nullptr) {
        co_return AdResult::NotInitialized;
    }
    if (displayCapper_->isCapped()) {
        co_return AdResult::Capped;
    }
    auto result = co_await ad_->show();
    displayCapper_->cap();
    co_return result;
}
} // namespace services
} // namespace ee
