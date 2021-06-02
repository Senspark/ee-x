//
//  GenericAd.cpp
//  Pods
//
//  Created by eps on 6/2/21.
//

#include "ee/services/internal/GenericAd.hpp"

#include <ee/ads/internal/ICapper.hpp>
#include <ee/ads/internal/IRetrier.hpp>
#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

namespace ee {
namespace services {
using Self = GenericAd;

Self::GenericAd(const std::shared_ptr<IAd>& ad,
                const std::shared_ptr<ads::ICapper>& loadCapper,
                const std::shared_ptr<ads::IRetrier>& loadRetrier)
    : ad_(ad)
    , loadCapper_(loadCapper)
    , loadRetrier_(loadRetrier) {
    handle_ = std::make_unique<ObserverHandle>();
    (*handle_).bind(*ad_).addObserver({
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
}

Self::~GenericAd() = default;

void Self::destroy() {
    ad_->destroy();
    handle_->clear();
    loadRetrier_->stop();
}

bool Self::isLoaded() const {
    return ad_->isLoaded();
}

Task<bool> Self::load() {
    auto result = co_await loadInternal();
    if (result) {
        loadRetrier_->stop();
    } else {
        noAwait([this]() -> Task<> {
            co_await loadRetrier_->process([this]() -> Task<bool> { //
                co_return co_await loadInternal();
            });
        });
    }
    co_return result;
}

Task<bool> Self::loadInternal() {
    if (loadCapper_->isCapped()) {
        co_return false;
    }
    loadCapper_->cap();
    co_return co_await ad_->load();
}
} // namespace services
} // namespace ee
