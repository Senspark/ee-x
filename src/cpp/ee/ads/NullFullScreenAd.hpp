#ifndef EE_X_NULL_FULL_SCREEN_AD_HPP
#define EE_X_NULL_FULL_SCREEN_AD_HPP

#ifdef __cplusplus

#include "ee/ads/IFullScreenAd.hpp"
#include "ee/ads/NullAd.hpp"

namespace ee {
namespace ads {
class NullFullScreenAd : public IFullScreenAd, public NullAd {
    virtual Task<AdResult> show() override;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_NULL_FULL_SCREEN_AD_HPP
