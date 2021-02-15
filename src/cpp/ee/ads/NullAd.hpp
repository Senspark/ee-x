//
//  NullAd.hpp
//  Pods
//
//  Created by eps on 2/15/21.
//

#ifndef EE_X_NULL_AD_HPP
#define EE_X_NULL_AD_HPP

#ifdef __cplusplus

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IAd.hpp"

namespace ee {
namespace ads {
class NullAd : public virtual IAd, public ObserverManager<AdObserver> {
public:
    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_NULL_AD_HPP */
