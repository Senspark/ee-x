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
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
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
    std::pair<int, int> getBannerAdSize(BannerAdSize adSize);

    std::shared_ptr<IFullScreenAd> createFullScreenAd(
        const std::string& handlerId, const std::string& adId,
        const std::function<std::shared_ptr<IFullScreenAd>()>& creator);

    bool destroyAd(const std::string& adId);

    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;
    std::map<std::string, std::shared_ptr<IAd>> ads_;
    std::shared_ptr<ads::IAsyncHelper<AdResult>> displayer_;
};
} // namespace facebook_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP */
