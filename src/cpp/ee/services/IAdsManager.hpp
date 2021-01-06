#ifndef EE_X_I_ADS_MANAGER_HPP
#define EE_X_I_ADS_MANAGER_HPP

#ifdef __cplusplus

#include "ee/services/ServicesFwd.hpp"

namespace ee {
namespace services {
enum class AdResult {
    NotInitialized,
    NotConfigured,
    Capped,
    NoInternet,
    NotLoaded,
    Failed,
    Canceled,
    Completed,
};

class IAdsManager {
public:
    virtual ~IAdsManager() = default;
    virtual bool isBannerAdVisible() const = 0;
    virtual void setBannerAdVisible(bool visible) = 0;
    virtual std::pair<float, float> getBannerAdAnchor() const = 0;
    virtual void setBannerAdAnchor(float x, float y) = 0;
    virtual std::pair<float, float> getBannerAdPosition() const = 0;
    virtual void setBannerAdPosition(float x, float y) = 0;
    virtual std::pair<float, float> getBannerAdSize() const = 0;
    virtual void setBannerAdSize(float x, float y) = 0;
    virtual Task<AdResult> showInterstitialAd() = 0;
    virtual Task<AdResult> showRewardedAd() = 0;
    virtual Task<AdResult> showAppOpenAd() = 0;
};
} // namespace services
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_ADS_MANAGER_HPP