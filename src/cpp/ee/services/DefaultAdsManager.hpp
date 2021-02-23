#ifndef EE_X_DEFAULT_ADS_MANAGER_HPP
#define EE_X_DEFAULT_ADS_MANAGER_HPP

#ifdef __cplusplus

#include <map>
#include <optional>
#include <string>

#include <ee/ads/AdsFwd.hpp>

#include "ee/services/IAdsManager.hpp"

namespace ee {
namespace services {
class DefaultAdsManager : public IAdsManager {
public:
    explicit DefaultAdsManager(const std::string& configJson);

    Task<bool> initialize();

    virtual std::shared_ptr<IBannerAd> getBannerAd() override;
    virtual std::shared_ptr<IBannerAd> getRectangleAd() override;
    virtual std::shared_ptr<IFullScreenAd> getAppOpenAd() override;
    virtual std::shared_ptr<IFullScreenAd> getInterstitialAd() override;
    virtual std::shared_ptr<IFullScreenAd> getRewardedInterstitialAd() override;
    virtual std::shared_ptr<IFullScreenAd> getRewardedAd() override;

private:
    Task<> initializeNetworks();
    void initializeBannerAd(AdFormat format);
    void initializeFullScreenAd(AdFormat format);

    bool initialized_;
    std::shared_ptr<ads::ICapper> displayCapper_;
    std::shared_ptr<AdsConfig> config_;
    std::map<AdFormat, std::shared_ptr<LazyBannerAd>> bannerAds_;
    std::map<AdFormat, std::shared_ptr<LazyFullScreenAd>> fullScreenAds_;
};
} // namespace services
} // namespace ee

#endif // __cplusplus

#endif // EE_X_DEFAULT_ADS_MANAGER_HPP
