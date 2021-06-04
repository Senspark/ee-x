//
//  GenericAd.cpp
//  ee-x-d542b565
//
//  Created by eps on 1/7/21.
//

#include "ee/services/internal/GenericFullScreenAd.hpp"

#include <cassert>

#include <ee/ads/internal/ICapper.hpp>
#include <ee/core/Delay.hpp>
#include <ee/core/Platform.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/WhenAll.hpp>

namespace ee {
namespace services {
using Self = GenericFullScreenAd;

Self::GenericFullScreenAd(const std::shared_ptr<IFullScreenAd>& ad,
                          const std::shared_ptr<ads::ICapper>& displayCapper,
                          const std::shared_ptr<ads::ICapper>& loadCapper,
                          const std::shared_ptr<ads::IRetrier>& loadRetrier)
    : GenericAd(ad, loadCapper, loadRetrier)
    , ad_(ad)
    , displayCapper_(displayCapper) {}

Self::~GenericFullScreenAd() = default;

Task<AdResult> Self::show() {
    if (displayCapper_->isCapped()) {
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
        displayCapper_->cap();
    }
    co_return result;
}

Task<bool> Self::testConnection(float timeOut) {
    co_return co_await Platform::testConnection("www.google.com", timeOut);
}
} // namespace services
} // namespace ee
