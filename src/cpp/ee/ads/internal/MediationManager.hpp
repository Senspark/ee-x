//
//  MediationManager.hpp
//  ee_x
//
//  Created by Zinge on 10/23/17.
//
//

#ifndef EE_X_ADS_MEDIATION_MANAGER_HPP
#define EE_X_ADS_MEDIATION_MANAGER_HPP

#include <functional>

#include "ee/ads/AdsFwd.hpp"

namespace ee {
namespace ads {
/// Fix issue where AdMob consumes other ads' callbacks.
class MediationManager {
private:
    using Self = MediationManager;

public:
    static Self& getInstance();

    const std::shared_ptr<IAsyncHelper<FullScreenAdResult>>&
    getAdDisplayer() const;

private:
    MediationManager();
    ~MediationManager();

    MediationManager(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    std::shared_ptr<IAsyncHelper<FullScreenAdResult>> adDisplayer_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_ADS_MEDIATION_MANAGER_HPP */
