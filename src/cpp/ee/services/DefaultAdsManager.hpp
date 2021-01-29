#ifndef EE_X_DEFAULT_ADS_MANAGER_HPP
#define EE_X_DEFAULT_ADS_MANAGER_HPP

#ifdef __cplusplus

#include <map>
#include <optional>
#include <string>

#include <ee/ads/AdsFwd.hpp>
#include <ee/core/ObserverManager.hpp>

#include "ee/services/IAdsManager.hpp"

namespace ee {
namespace services {
class DefaultAdsManager : public IAdsManager,
                          public ObserverManager<AdObserver> {
public:
    explicit DefaultAdsManager(const std::string& configJson);

    Task<bool> initialize();

    virtual bool isBannerAdLoaded() const override;
    virtual bool isBannerAdVisible() const override;
    virtual void setBannerAdVisible(bool visible) override;
    virtual std::pair<float, float> getBannerAdAnchor() const override;
    virtual void setBannerAdAnchor(float x, float y) override;
    virtual std::pair<float, float> getBannerAdPosition() const override;
    virtual void setBannerAdPosition(float x, float y) override;
    virtual std::pair<float, float> getBannerAdSize() const override;
    virtual void setBannerAdSize(float width, float height) override;

    virtual bool isRectangleAdLoaded() const override;
    virtual bool isRectangleAdVisible() const override;
    virtual void setRectangleAdVisible(bool visible) override;
    virtual std::pair<float, float> getRectangleAdAnchor() const override;
    virtual void setRectangleAdAnchor(float x, float y) override;
    virtual std::pair<float, float> getRectangleAdPosition() const override;
    virtual void setRectangleAdPosition(float x, float y) override;
    virtual std::pair<float, float> getRectangleAdSize() const override;
    virtual void setRectangleAdSize(float width, float height) override;

    virtual Task<AdResult> showAppOpenAd() override;
    virtual Task<AdResult> showInterstitialAd() override;
    virtual Task<AdResult> showRewardedInterstitialAd() override;
    virtual Task<AdResult> showRewardedAd() override;

private:
    Task<> initializeNetworks();
    void initializeBannerAd(AdFormat format);
    void initializeFullScreenAd(AdFormat format);
    Task<AdResult> showFullScreenAd(AdFormat format);

    bool initialized_;
    bool fullScreenAdCapped_;
    std::shared_ptr<AdsConfig> config_;
    std::map<AdFormat, std::shared_ptr<LazyBannerAd>> bannerAds_;
    std::map<AdFormat, std::shared_ptr<GenericAd>> fullScreenAds_;

    std::unique_ptr<ObserverHandle> handle_;
};
} // namespace services
} // namespace ee

#endif // __cplusplus

#endif // EE_X_DEFAULT_ADS_MANAGER_HPP
