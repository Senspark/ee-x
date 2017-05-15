//
//  FacebookAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_FACEBOOK_ADS_BRIDGE_HPP_
#define EE_X_FACEBOOK_ADS_BRIDGE_HPP_
#include <memory>
#include <string>

namespace ee {
namespace facebook {
class FacebookAds final{
public:
    void initFBAds(const std::string& InterstitialID, const std::string& NativeID, const std::string& BannerID) const;
    void cacheRewardedAd(const std::string& adsID) const;
    void cacheInterstitialAd(const std::string& adsID) const;
    
    void showBannerAd(const std::string& adsID, int pos) const;
    void hideBannerAd() const;
    
    bool hasInterstitialAd() const;
    bool hasRewardedAd() const;
    
    void showInterstitialAd() const;
    void showRewardedAd() const;
    
    void showNativeExpressAd(const std::string& adsID, int width,
                                     int height, int pos);
    void showNativeExpressAdWithDeltaPosition(const std::string& adsId,
                                              int width, int height, int deltaX,
                                              int deltaY);
    void hideNativeExpressAd();
    
    void showNativeAdvancedAd(const std::string& adsID, int width,
                                      int height, int x, int y);
    
    void hideNativeAdvancedAd();
    
    void onAdsCallback(int code, const std::string& msg) const;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
