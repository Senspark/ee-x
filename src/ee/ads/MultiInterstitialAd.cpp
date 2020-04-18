//
//  MultiInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/ads/MultiInterstitialAd.hpp"

#include <ee/core/ObserverHandle.hpp>
#include <ee/coroutine/Task.hpp>

namespace ee {
namespace ads {
using Self = MultiInterstitialAd;

Self::MultiInterstitialAd() {
    handle_ = std::make_unique<ObserverHandle>();
}

Self& Self::addItem(const std::shared_ptr<IInterstitialAd>& item) {
    items_.push_back(item);
    (*handle_).bind(*item).addObserver({
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
    return *this;
}

void Self::destroy() {
    for (auto&& item : items_) {
        item->destroy();
    }
    items_.clear();
    handle_->clear();
}

bool Self::isLoaded() const {
    for (auto&& item : items_) {
        if (item->isLoaded()) {
            return true;
        }
    }
    return false;
}

Task<bool> Self::load() {
    bool result = false;
    for (auto&& item : items_) {
        if (item->isLoaded()) {
            continue;
        }
        if (co_await item->load()) {
            result = true;
        }
    }
    co_return result;
}

Task<bool> Self::show() {
    bool displayed = false;
    for (auto&& item : items_) {
        if (not displayed) {
            if (co_await item->show()) {
                displayed = true;
                continue;
            }
        }
        if (not item->isLoaded()) {
            item->load();
        }
    }
    co_return displayed;
}
} // namespace ads
} // namespace ee
