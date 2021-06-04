//
//  GenericAd.hpp
//  ee-x-d542b565
//
//  Created by eps on 1/7/21.
//

#ifndef EE_X_GENERIC_FULL_SCREEN_AD_HPP
#define EE_X_GENERIC_FULL_SCREEN_AD_HPP

#include <ee/ads/IFullScreenAd.hpp>

#include "ee/services/internal/GenericAd.hpp"

namespace ee {
namespace services {
class GenericFullScreenAd : public IFullScreenAd, public GenericAd {
public:
    explicit GenericFullScreenAd(
        const std::shared_ptr<IFullScreenAd>& ad,
        const std::shared_ptr<ads::ICapper>& displayCapper,
        const std::shared_ptr<ads::ICapper>& loadCapper,
        const std::shared_ptr<ads::IRetrier>& loadRetrier);
    virtual ~GenericFullScreenAd() override;

    virtual Task<AdResult> show() override;

private:
    static Task<bool> testConnection(float timeOut);

    std::shared_ptr<IFullScreenAd> ad_;
    std::shared_ptr<ads::ICapper> displayCapper_;
};
} // namespace services
} // namespace ee

#endif /* EE_X_GENERIC_FULL_SCREEN_AD_HPP */
