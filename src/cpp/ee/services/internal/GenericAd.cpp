//
//  GenericAd.cpp
//  ee-x-d542b565
//
//  Created by eps on 1/7/21.
//

#include "ee/services/internal/GenericAd.hpp"

#include <cassert>

#include <ee/core/Delay.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Platform.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/WhenAll.hpp>

#include "ee/ads/internal/Capper.hpp"

namespace ee {
namespace services {
using Self = GenericAd;

Self::GenericAd(const std::shared_ptr<IFullScreenAd>& ad, int interval)
    : ad_(ad) {
    capper_ = std::make_shared<ads::Capper>(interval);
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
}

bool Self::isLoaded() const {
    return ad_->isLoaded();
}

Task<bool> Self::load() {
    auto result = co_await ad_->load();
    co_return result;
}

Task<AdResult> Self::show() {
    if (capper_->isCapped()) {
        co_return AdResult::Capped;
    }
    if (ad_->isLoaded()) {
        auto hasInternet = co_await testConnection(0.2f);
        if (hasInternet) {
            // OK.
        } else {
            co_return AdResult::NoInternet;
        }
    } else {
        noAwait(ad_->load());
        auto hasInternet = false;
        co_await whenAll(    //
            []() -> Task<> { //
                co_await Delay(0.1f);
            },
            [&hasInternet]() -> Task<> {
                hasInternet = co_await testConnection(0.2f);
            });
        if (hasInternet) {
            // OK.
        } else {
            co_return AdResult::NoInternet;
        }
    }
    if (ad_->isLoaded()) {
        // OK.
    } else {
        co_return AdResult::NotLoaded;
    }
    auto result = co_await ad_->show();
    noAwait(ad_->load());
    if (result == AdResult::Completed) {
        capper_->cap();
    }
    co_return result;
}

Task<bool> Self::testConnection(float timeOut) {
    co_return co_await Platform::testConnection("www.google.com", timeOut);
}
} // namespace services
} // namespace ee
