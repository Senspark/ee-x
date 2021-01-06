#ifndef EE_X_NULL_FULL_SCREEN_AD_HPP
#define EE_X_NULL_FULL_SCREEN_AD_HPP

#ifdef __cplusplus

#include <ee/core/ObserverManager.hpp>

#include "ee/ads/IFullScreenAd.hpp"

namespace ee {
namespace ads {
class NullFullScreenAd : public IFullScreenAd,
                         public ObserverManager<AdObserver> {
    virtual void destroy() override;

    virtual bool isLoaded() const override;
    virtual Task<bool> load() override;
    virtual Task<FullScreenAdResult> show() override;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_NULL_FULL_SCREEN_AD_HPP