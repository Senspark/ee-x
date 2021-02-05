#ifndef EE_X_I_ADS_MANAGER_HPP
#define EE_X_I_ADS_MANAGER_HPP

#ifdef __cplusplus

#include <functional>

#include <ee/core/IObserverManager.hpp>

#include "ee/services/ServicesFwd.hpp"

namespace ee {
namespace services {
struct AdsObserver {
    std::function<void()> onClicked;
};

class IAdsManager : public virtual IObserverManager<AdObserver> {
public:
    virtual ~IAdsManager() = default;
    virtual bool isBannerAdLoaded() const = 0;
    virtual bool isBannerAdVisible() const = 0;
    virtual void setBannerAdVisible(bool visible) = 0;
    virtual std::pair<float, float> getBannerAdAnchor() const = 0;
    virtual void setBannerAdAnchor(float x, float y) = 0;
    virtual std::pair<float, float> getBannerAdPosition() const = 0;
    virtual void setBannerAdPosition(float x, float y) = 0;
    virtual std::pair<float, float> getBannerAdSize() const = 0;
    virtual void setBannerAdSize(float x, float y) = 0;

    virtual bool isRectangleAdLoaded() const = 0;
    virtual bool isRectangleAdVisible() const = 0;
    virtual void setRectangleAdVisible(bool visible) = 0;
    virtual std::pair<float, float> getRectangleAdAnchor() const = 0;
    virtual void setRectangleAdAnchor(float x, float y) = 0;
    virtual std::pair<float, float> getRectangleAdPosition() const = 0;
    virtual void setRectangleAdPosition(float x, float y) = 0;
    virtual std::pair<float, float> getRectangleAdSize() const = 0;
    virtual void setRectangleAdSize(float x, float y) = 0;

    virtual Task<AdResult> showAppOpenAd() = 0;
    virtual Task<AdResult> showInterstitialAd() = 0;
    virtual Task<AdResult> showRewardedInterstitialAd() = 0;
    virtual Task<AdResult> showRewardedAd() = 0;
};
} // namespace services
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_ADS_MANAGER_HPP
