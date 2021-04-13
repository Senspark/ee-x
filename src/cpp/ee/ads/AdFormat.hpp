#ifndef EE_X_AD_FORMAT_HPP
#define EE_X_AD_FORMAT_HPP

#ifdef __cplusplus

#include "ee/ads/AdsFwd.hpp"

namespace ee {
namespace ads {
enum class AdFormat {
    Banner,
    Rectangle,
    AppOpen,
    Interstitial,
    RewardedInterstitial,
    Rewarded,
    Null,
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_AD_FORMAT_HPP */
