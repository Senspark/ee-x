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

Self::~MultiInterstitialAd() {
    for (auto&& item : items_) {
        item->setResultCallback(nullptr);
    }
}

Self& Self::addItem(const std::shared_ptr<InterstitialAdInterface>& item) {
    items_.push_back(item);
    item->setResultCallback(std::bind(&Self::setDone, this));
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
    for (auto&& item : items_) {
        if (item->show()) {
            return true;
        }
    }
    return false;
}
} // namespace ads
} // namespace ee
