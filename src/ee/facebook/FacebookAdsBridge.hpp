//
//  FacebookAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_FACEBOOK_ADS_BRIDGE_HPP_
#define EE_X_FACEBOOK_ADS_BRIDGE_HPP_

#include <functional>
#include <memory>
#include <string>

namespace ee {
namespace facebook {
class FacebookAds final {
public:
    using AdCallback = std::function<void(const std::string& message)>;

    FacebookAds();
    ~FacebookAds();

    void initFBAdsInterstitial(const std::string& interstitialAdId) const;

    void initFBAdsNativeAds(const std::string& nativeAdId,
                            const std::string& layout) const;

    void initFBAdsBanner(const std::string& bannerAdId) const;

    void cacheRewardedAd(const std::string& adId) const;
    void cacheInterstitialAd(const std::string& adId) const;

    void showBannerAd(const std::string& adId, int position) const;
    void hideBannerAd() const;

    bool hasInterstitialAd() const;
    bool hasRewardedAd() const;
    bool hasNativeAd(const std::string& adId) const;

    void showInterstitialAd() const;
    void showRewardedAd() const;

    void hideNativeAd(const std::string& adId);
    void showNativeAd(const std::string& adId, int width, int height, int x,
                      int y);

    void setCallback(const AdCallback& callback) { callback_ = callback; }
    const AdCallback& getCallback() const { return callback_; }

private:
    std::string onAdCallback(const std::string& msg) const;

    AdCallback callback_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
