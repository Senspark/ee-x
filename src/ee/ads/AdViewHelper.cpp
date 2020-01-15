//
//  AdViewHelper.cpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#include "ee/ads/AdViewHelper.hpp"

namespace ee {
namespace ads {
using Self = AdViewHelper;

Self::AdViewHelper(const std::string& prefix, const std::string& adId) {
    prefix_ = prefix;
    adId_ = adId;
}

std::string Self::k__isLoaded() const {
    return prefix_ + "_isLoaded_" + adId_;
}

std::string Self::k__load() const {
    return prefix_ + "_load_" + adId_;
}

std::string Self::k__getPosition() const {
    return prefix_ + "_getPosition_" + adId_;
}

std::string Self::k__setPosition() const {
    return prefix_ + "_setPosition_" + adId_;
}

std::string Self::k__getSize() const {
    return prefix_ + "_getSize_" + adId_;
}

std::string Self::k__setSize() const {
    return prefix_ + "_setSize_" + adId_;
}

std::string Self::k__setVisible() const {
    return prefix_ + "_setVisible_" + adId_;
}
} // namespace ads
} // namespace ee
