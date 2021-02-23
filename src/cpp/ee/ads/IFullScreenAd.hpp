#ifndef EE_X_I_FULL_SCREEN_AD_HPP
#define EE_X_I_FULL_SCREEN_AD_HPP

#ifdef __cplusplus

#include "ee/ads/IAd.hpp"

namespace ee {
namespace ads {
enum class AdResult {
    /// Used by DefaultFullScreenAd.
    Failed,
    Canceled,
    Completed,

    /// Used by LazyFullScreenAd.
    NotInitialized,
    NotConfigured,

    /// Used by GenericAd.
    Capped,
    NoInternet,
    NotLoaded,
};

class IFullScreenAd : public virtual IAd {
public:
    /// Attempts to show this ad.
    [[nodiscard]] virtual Task<AdResult> show() = 0;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_FULL_SCREEN_AD_HPP
