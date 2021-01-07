#ifndef EE_X_DEFAULT_ADS_MANAGER_HPP
#define EE_X_DEFAULT_ADS_MANAGER_HPP

#ifdef __cplusplus

#include <map>
#include <optional>
#include <string>

#include "ee/services/IAdsManager.hpp"

namespace ee {
namespace services {
class DefaultAdsManager : public IAdsManager {
public:
    explicit DefaultAdsManager(const std::string& configJson);

    Task<bool> initialize();

    virtual bool isBannerAdVisible() const override;
    virtual void setBannerAdVisible(bool visible) override;
    virtual std::pair<float, float> getBannerAdAnchor() const override;
    virtual void setBannerAdAnchor(float x, float y) override;
    virtual std::pair<float, float> getBannerAdPosition() const override;
    virtual void setBannerAdPosition(float x, float y) override;
    virtual std::pair<float, float> getBannerAdSize() const override;
    virtual void setBannerAdSize(float width, float height) override;
    virtual Task<AdResult> showAppOpenAd() override;
    virtual Task<AdResult> showInterstitialAd() override;
    virtual Task<AdResult> showRewardedAd() override;

private:
    Task<> initializeNetworks();
    void initializeBannerAd();
    void initializeAppOpenAd();
    void initializeInterstitialAd();
    void initializeRewardedAd();
    
    static Task<bool> testConnection(float timeOut);

    bool initialized_;
    std::shared_ptr<AdsConfig> config_;
    std::shared_ptr<IAdView> bannerAd_;
    std::shared_ptr<IFullScreenAd> appOpenAd_;
    std::shared_ptr<IFullScreenAd> interstitialAd_;
    std::shared_ptr<IFullScreenAd> rewardedAd_;
    bool isAppOpenAdCapped_;
    int appOpenAdInterval_;
    bool isInterstitialAdCapped_;
    int interstitialAdInterval_;
    bool isBannerAdVisible_;
    std::pair<float, float> bannerAdAnchor_;
    std::pair<float, float> bannerAdPosition_;
    std::optional<std::pair<float, float>> bannerAdSize_;
};
} // namespace services
} // namespace ee

#endif // __cplusplus

#endif // EE_X_DEFAULT_ADS_MANAGER_HPP
