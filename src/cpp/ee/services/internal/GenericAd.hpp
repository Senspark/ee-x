//
//  GenericAd.hpp
//  Pods
//
//  Created by eps on 6/2/21.
//

#ifndef EE_X_GENERIC_AD_HPP
#define EE_X_GENERIC_AD_HPP

#include <ee/ads/IAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/services/ServicesFwd.hpp"

namespace ee {
namespace services {
class GenericAd : public virtual IAd, public ObserverManager<AdObserver> {
public:
    explicit GenericAd(const std::shared_ptr<IAd>& ad,
                       const std::shared_ptr<ads::ICapper>& loadCapper,
                       const std::shared_ptr<ads::IRetrier>& loadRetrier);

    virtual ~GenericAd() override;

    virtual void destroy() override final;

    virtual bool isLoaded() const override final;
    virtual Task<bool> load() override final;

private:
    virtual Task<bool> loadInternal();

    std::shared_ptr<IAd> ad_;
    std::shared_ptr<ads::ICapper> loadCapper_;
    std::shared_ptr<ads::IRetrier> loadRetrier_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace services
} // namespace ee

#endif /* EE_X_GENERIC_AD_HPP */
