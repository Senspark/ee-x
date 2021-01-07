#ifndef EE_X_I_AD_HPP
#define EE_X_I_AD_HPP

#ifdef __cplusplus

#include <functional>

#include <ee/core/IObserverManager.hpp>

#include "ee/ads/AdsFwd.hpp"

namespace ee {
namespace ads {
struct AdObserver {
    /// Occurs when this ad is loaded.
    std::function<void()> onLoaded;

    /// Occurs when the user clicks this ad.
    std::function<void()> onClicked;
};

class IAd : public virtual IObserverManager<AdObserver> {
public:
    /// Destroys this ad.
    virtual void destroy() = 0;

    /// Checks whether this ad is loaded.
    virtual bool isLoaded() const = 0;

    /// Attempts to load this ad.
    [[nodiscard]] virtual Task<bool> load() = 0;
};
} // namespace ads
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_AD_HPP