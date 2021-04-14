#ifndef EE_X_I_ADS_MANAGER_HPP
#define EE_X_I_ADS_MANAGER_HPP

#ifdef __cplusplus

#include <memory>

#include "ee/services/ServicesFwd.hpp"

namespace ee {
namespace services {
class IAdsManager {
public:
    virtual ~IAdsManager() = default;
    virtual void addTestDevice(const std::string& hash) = 0;
    virtual void openTestSuite() = 0;
    virtual std::shared_ptr<IBannerAd> getBannerAd() = 0;
    virtual std::shared_ptr<IBannerAd> getRectangleAd() = 0;
    virtual std::shared_ptr<IFullScreenAd> getAppOpenAd() = 0;
    virtual std::shared_ptr<IFullScreenAd> getInterstitialAd() = 0;
    virtual std::shared_ptr<IFullScreenAd> getRewardedInterstitialAd() = 0;
    virtual std::shared_ptr<IFullScreenAd> getRewardedAd() = 0;
};
} // namespace services
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_ADS_MANAGER_HPP
