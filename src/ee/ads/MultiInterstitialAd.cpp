//
//  MultiInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/ads/MultiInterstitialAd.hpp"

namespace ee {
namespace ads {
using Self = MultiInterstitialAd;

Self::MultiInterstitialAd() {}

Self::~MultiInterstitialAd() {}

Self& Self::addItem(const std::shared_ptr<IInterstitialAd>& item) {
    items_.push_back(item);
    return *this;
}

bool Self::isLoaded() const {
    for (auto&& item : items_) {
        if (item->isLoaded()) {
            return true;
        }
    }
    return false;
}

void Self::load() {
    for (auto&& item : items_) {
        if (not item->isLoaded()) {
            item->load();
        }
    }
}

bool Self::show() {
    bool displayed = false;

    // Assign callbacks for all items (fix AdMob mediation consumes callback
    // from other ads).
    assignCallbacks();

    for (auto&& item : items_) {
        if (not displayed) {
            if (item->show()) {
                displayed = true;
                continue;
            }
        }
        if (not item->isLoaded()) {
            item->load();
        }
    }

    if (not displayed) {
        clearCallbacks();
    }
    return displayed;
}

void Self::assignCallbacks() {
    for (auto&& item : items_) {
        item->setResultCallback([this] {
            setDone();
            clearCallbacks();
        });
    }
}

void Self::clearCallbacks() {
    for (auto&& item : items_) {
        item->setResultCallback(nullptr);
    }
}
} // namespace ads
} // namespace ee
