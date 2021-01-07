//
//  AdMob.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_ADMOB_BRIDGE_HPP
#define EE_X_ADMOB_BRIDGE_HPP

#include <map>

#include "ee/ad_mob/IAdMobBridge.hpp"

namespace ee {
namespace admob {
class Bridge final : public IBridge {
public:
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual Task<bool> initialize() override;
    virtual std::string getEmulatorTestDeviceHash() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual std::shared_ptr<IAdView>
    createBannerAd(const std::string& adId, BannerAdSize adSize) override;
    virtual std::shared_ptr<IAdView>
    createNativeAd(const std::string& adId, const std::string& layoutName,
                   const NativeAdLayout& identifiers) override;
    virtual std::shared_ptr<IFullScreenAd>
    createAppOpenAd(const std::string& adId) override;
    virtual std::shared_ptr<IFullScreenAd>
    createInterstitialAd(const std::string& adId) override;
    virtual std::shared_ptr<IFullScreenAd>
    createRewardedAd(const std::string& adId) override;

private:
    friend BannerAd;
    friend NativeAd;
    friend InterstitialAd;
    friend RewardedAd;
    friend AppOpenAd;

    std::pair<int, int> getBannerAdSize(BannerAdSize adSize);

    template <class Ad>
    std::shared_ptr<IFullScreenAd>
    createFullScreenAd(const std::string& handlerId, const std::string& adId);

    bool destroyBannerAd(const std::string& adId);
    bool destroyNativeAd(const std::string& adId);
    bool destroyAppOpenAd(const std::string& adId);
    bool destroyInterstitialAd(const std::string& adId);
    bool destroyRewardedAd(const std::string& adId);
    bool destroyAd(const std::string& handlerId, const std::string& adId);

    IMessageBridge& bridge_;
    const Logger& logger_;
    std::map<std::string, std::shared_ptr<IAd>> ads_;
    std::shared_ptr<ads::IAsyncHelper<FullScreenAdResult>> displayer_;
};
} // namespace admob
} // namespace ee

#endif /* EE_X_ADMOB_BRIDGE_HPP */
