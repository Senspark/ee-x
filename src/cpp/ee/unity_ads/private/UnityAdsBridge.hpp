//
//  UnityAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_UNITY_ADS_BRIDGE_HPP
#define EE_X_UNITY_ADS_BRIDGE_HPP

#include <map>

#include "ee/unity_ads/IUnityAdsBridge.hpp"

namespace ee {
namespace unity_ads {
class Bridge final : public IBridge {
public:
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual Task<bool> initialize(const std::string& gameId,
                                  bool testModeEnabled) override;
    virtual void setDebugModeEnabled(bool enabled) override;
    virtual std::shared_ptr<IInterstitialAd>
    createInterstitialAd(const std::string& adId) override;
    virtual std::shared_ptr<IRewardedAd>
    createRewardedAd(const std::string& adId) override;

private:
    friend InterstitialAd;
    friend RewardedAd;

    bool destroyInterstitialAd(const std::string& adId);
    bool destroyRewardedAd(const std::string& adId);

    bool hasRewardedAd(const std::string& adId) const;
    Task<bool> loadRewardedAd(const std::string& adId);
    void showRewardedAd(const std::string& adId);

    void onLoaded(const std::string& adId);
    void onFailedToShow(const std::string& adId, const std::string& message);
    void onClosed(const std::string& adId, bool rewarded);

    void onMediationAdFailedToShow(const std::string& adId,
                                   const std::string& message);
    void onMediationAdClosed(const std::string& adId, bool rewarded);

    IMessageBridge& bridge_;
    const Logger& logger_;

    bool displaying_;

    /// Currently displaying ad ID.
    std::string adId_;

    template <class Ad, class Raw>
    struct Entry {
        std::shared_ptr<Ad> ad;
        std::shared_ptr<Raw> raw;

        explicit Entry(const std::shared_ptr<Ad>& ad_,
                       const std::shared_ptr<Raw>& raw_)
            : ad(ad_)
            , raw(raw_) {}
    };

    /// Unity only has rewarded ads.
    std::map<std::string, Entry<IInterstitialAd, InterstitialAd>>
        interstitialAds_;
    std::map<std::string, Entry<IRewardedAd, RewardedAd>> rewardedAds_;

    std::shared_ptr<ads::IAsyncHelper<bool>> interstitialAdDisplayer_;
    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>> rewardedAdDisplayer_;
};
} // namespace unity_ads
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
