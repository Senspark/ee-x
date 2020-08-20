//
//  AdMob.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#ifndef EE_X_ADMOB_BRIDGE_HPP
#define EE_X_ADMOB_BRIDGE_HPP

#ifdef __cplusplus

#include <map>

#include "ee/admob/IAdMobBridge.hpp"

namespace ee {
namespace admob {
class Bridge final : public IBridge {
public:
    Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual void initialize() override;
    virtual std::string getEmulatorTestDeviceHash() const override;
    virtual void addTestDevice(const std::string& hash) override;
    virtual std::shared_ptr<IAdView>
    createBannerAd(const std::string& adId, BannerAdSize adSize) override;
    virtual std::shared_ptr<IAdView>
    createNativeAd(const std::string& adId, const std::string& layoutName,
                   const NativeAdLayout& identifiers) override;
    virtual std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId) override;
    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) override;

private:
    friend BannerAd;
    friend NativeAd;
    friend InterstitialAd;
    friend RewardedAd;

    std::pair<int, int> getBannerAdSize(BannerAdSize adSize);

    bool destroyBannerAd(const std::string& adId);
    bool destroyNativeAd(const std::string& adId);
    bool destroyInterstitialAd(const std::string& adId);
    bool destroyRewardedAd(const std::string& adId);

    IMessageBridge& bridge_;
    const Logger& logger_;

    std::map<std::string, std::shared_ptr<IAdView>> bannerAds_;
    std::map<std::string, std::shared_ptr<IAdView>> nativeAds_;
    std::map<std::string, std::shared_ptr<IInterstitialAd>> interstitialAds_;
    std::map<std::string, std::shared_ptr<IRewardedAd>> rewardedAds_;

    std::shared_ptr<ads::IAsyncHelper<bool>> interstitialAdDisplayer_;
    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> rewardedAdDisplayer_;
};
} // namespace admob
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADMOB_BRIDGE_HPP */
