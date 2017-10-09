//
//  AdViewInterface.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include "AdViewInterface.hpp"

namespace ee {
namespace ads {
using Self = AdViewInterface;

Self::AdViewInterface(const std::string& adId)
    : adId_(adId) {}

Self::~AdViewInterface() {}

const std::string& Self::getAdId() const {
    return adId_;
}
} // namespace ads
} // namespace ee
