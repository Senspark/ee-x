//
//  IInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#ifndef EE_X_I_INTERSTITIAL_AD_HPP
#define EE_X_I_INTERSTITIAL_AD_HPP

#ifdef __cplusplus

#include <functional>

#include <ee/core/IObserverManager.hpp>

#include "ee/ads/AdsFwd.hpp"

namespace ee {
namespace ads {
struct IInterstitialAdObserver {
    /// Occurs when this ad is loaded.
    std::function<void()> onLoaded;

    /// Occurs when the user clicks this ad.
    std::function<void()> onClicked;
};

class IInterstitialAd
    : public virtual IObserverManager<IInterstitialAdObserver> {
public:
    virtual ~IInterstitialAd() = default;

    /// Destroys this ad.
    virtual void destroy() = 0;

    /// Checks whether this ad is loaded.
    virtual bool isLoaded() const = 0;

    /// Attempts to load this ad.
    [[nodiscard]] virtual Task<bool> load() = 0;

    /// Attempts to show this ad.
    [[nodiscard]] virtual Task<bool> show() = 0;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_INTERSTITIAL_AD_HPP */
