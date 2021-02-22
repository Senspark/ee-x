//
//  LazyFullScreenAd.hpp
//  Pods
//
//  Created by eps on 2/22/21.
//

#ifndef EE_X_LAZY_FULL_SCREEN_AD_HPP
#define EE_X_LAZY_FULL_SCREEN_AD_HPP

#include <ee/ads/IFullScreenAd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/services/ServicesFwd.hpp"

namespace ee {
namespace services {
class LazyFullScreenAd : public IFullScreenAd,
                         public ObserverManager<AdObserver> {
public:
    explicit LazyFullScreenAd(
        const std::shared_ptr<ads::ICapper>& displayCapper);
    virtual ~LazyFullScreenAd() override;

    void setAd(const std::shared_ptr<IFullScreenAd>& ad);

    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<AdResult> show() override;

private:
    std::shared_ptr<IFullScreenAd> ad_;
    std::shared_ptr<ads::ICapper> displayCapper_;
    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace services
} // namespace ee

#endif /* EE_X_LAZY_FULL_SCREEN_AD_HPP */
