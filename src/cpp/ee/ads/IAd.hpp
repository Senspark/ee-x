#ifndef EE_X_I_AD_HPP
#define EE_X_I_AD_HPP

#ifdef __cplusplus

#include <functional>

#include <ee/core/IObserverManager.hpp>

#include "ee/ads/AdsFwd.hpp"

namespace ee {
namespace ads {
struct AdLoadResult {
    std::string network;
    bool result;
    int errorCode;
    std::string errorMessage;
};

struct AdPaidResult {
    std::string adPlatform;
    std::string networkName;
    std::string adUnitId;
    std::string adFormat;
    double revenue;
};

struct AdObserver {
    /// Occurs when this ad is loaded.
    std::function<void()> onLoaded;

    /// Occurs when this ad finishes loading.
    std::function<void(const AdLoadResult& result)> onLoadResult;

    /// Occurs when the user clicks this ad.
    std::function<void()> onClicked;

    std::function<void(const AdPaidResult& result)> onAdPaid;
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
