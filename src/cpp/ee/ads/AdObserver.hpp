#ifndef EE_X_AD_OBSERVER_HPP
#define EE_X_AD_OBSERVER_HPP

#ifdef __cplusplus

#include <functional>

#include "ee/ads/AdsFwd.hpp"

namespace ee {
namespace ads {
struct AdObserver {
    /// Occurs when this ad is loaded.
    std::function<void()> onLoaded;

    /// Occurs when the user clicks this ad.
    std::function<void()> onClicked;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_AD_OBSERVER_HPP