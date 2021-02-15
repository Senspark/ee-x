#ifndef EE_X_I_FULL_SCREEN_AD_HPP
#define EE_X_I_FULL_SCREEN_AD_HPP

#ifdef __cplusplus

#include "ee/ads/IAd.hpp"

namespace ee {
namespace ads {
enum class FullScreenAdResult {
    /// Failed to display the ad.
    Failed,

    /// Succeeded to display the ad and the user has canceled the ad.
    Canceled,

    /// Succeeded to display the ad and the user has completed the ad.
    Completed,
};

class IFullScreenAd : public virtual IAd {
public:
    /// Attempts to show this ad.
    [[nodiscard]] virtual Task<FullScreenAdResult> show() = 0;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_FULL_SCREEN_AD_HPP
