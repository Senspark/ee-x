//
//  FacebookAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_FACEBOOK_ADS_BRIDGE_HPP
#define EE_X_FACEBOOK_ADS_BRIDGE_HPP

#include <map>

#include "ee/facebook_ads/IFacebookAdsBridge.hpp"

namespace ee {
namespace facebook_ads {
class Bridge final : public IBridge {
public:
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual Task<bool> initialize() override;
    virtual std::string getTestDeviceHash() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual void clearTestDevices() override;
    virtual std::shared_ptr<IBannerAd>
    createBannerAd(const std::string& adId, BannerAdSize adSize) override;
    virtual std::shared_ptr<IBannerAd>
    createNativeAd(const std::string& adId, const std::string& layoutName,
                   const NativeAdLayout& identifiers) override;
    virtual std::shared_ptr<IFullScreenAd>
    createInterstitialAd(const std::string& adId) override;
    virtual std::shared_ptr<IFullScreenAd>
    createRewardedAd(const std::string& adId) override;

private:
    friend BannerAd;
    friend NativeAd;
    friend InterstitialAd;
    friend RewardedAd;

    std::pair<int, int> getBannerAdSize(BannerAdSize adSize);

    template <class Ad>
    std::shared_ptr<IFullScreenAd>
    createFullScreenAd(const std::string& handlerId, const std::string& adId);

    bool destroyBannerAd(const std::string& adId);
    bool destroyNativeAd(const std::string& adId);
    bool destroyInterstitialAd(const std::string& adId);
    bool destroyRewardedAd(const std::string& adId);
    bool destroyAd(const std::string& handlerId, const std::string& adId);

    IMessageBridge& bridge_;
    const Logger& logger_;
    std::map<std::string, std::shared_ptr<IAd>> ads_;
    std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>> displayer_;
};
} // namespace facebook_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP */
