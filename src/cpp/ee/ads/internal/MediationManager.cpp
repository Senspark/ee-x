
//
//  MediationManager.cpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#include "ee/ads/internal/MediationManager.hpp"

#include "ee/ads/internal/AsyncHelper.hpp"

namespace ee {
namespace ads {
using Self = MediationManager;

Self& Self::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

Self::MediationManager() {
    adDisplayer_ = std::make_shared<AsyncHelper<AdResult>>();
}

Self::~MediationManager() {}

const std::shared_ptr<IAsyncHelper<AdResult>>& Self::getAdDisplayer() const {
    return adDisplayer_;
}
} // namespace ads
} // namespace ee
