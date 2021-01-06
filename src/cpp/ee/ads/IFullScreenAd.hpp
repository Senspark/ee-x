#ifndef EE_X_I_FULL_SCREEN_AD_HPP
#define EE_X_I_FULL_SCREEN_AD_HPP

#ifdef __cplusplus

#include <ee/core/IObserverManager.hpp>

#include "ee/ads/AdObserver.hpp"

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

class IFullScreenAd : public virtual IObserverManager<AdObserver> {
public:
    virtual ~IFullScreenAd() = default;

    /// Destroys this ad.
    virtual void destroy() = 0;

    /// Checks whether this ad is loaded.
    virtual bool isLoaded() const = 0;

    /// Attempts to load this ad.
    [[nodiscard]] virtual Task<bool> load() = 0;

    /// Attempts to show this ad.
    [[nodiscard]] virtual Task<FullScreenAdResult> show() = 0;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_FULL_SCREEN_AD_HPP
