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

Self& Self::addItem(const std::shared_ptr<InterstitialAdInterface>& item) {
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
    for (auto&& item : items_) {
        if (not displayed) {
            item->setResultCallback([this, item] {
                setDone();
                item->setResultCallback(nullptr);
            });
            if (item->show()) {
                displayed = true;
                continue;
            }
        }
        if (not item->isLoaded()) {
            item->load();
        }
    }
    return displayed;
}
} // namespace ads
} // namespace ee
