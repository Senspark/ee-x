//
//  GuardedAd.hpp
//  Pods
//
//  Created by eps on 6/2/21.
//

#ifndef EE_X_GUARDED_AD_HPP
#define EE_X_GUARDED_AD_HPP

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IAd.hpp"

namespace ee {
namespace ads {
class GuardedAd : public virtual IAd, public ObserverManager<AdObserver> {
public:
    explicit GuardedAd(const std::shared_ptr<IAd>& ad);
    virtual ~GuardedAd() override;

    virtual void destroy() override final;
    virtual bool isLoaded() const override final;
    virtual Task<bool> load() override final;

protected:
    bool isLoading() const;
    virtual bool isDisplaying() const = 0;

    bool loaded_;

private:
    std::shared_ptr<IAd> ad_;
    bool loading_;

    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace ads
} // namespace ee

#endif /* EE_X_GUARDED_AD_HPP */
