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
#include <functional>

namespace ee {
namespace facebook {
class FacebookAds final{
public:
    FacebookAds();
    ~FacebookAds();
    
    typedef std::function<void(const std::string&)> AdsCallback;
    
    void initFBAdsInterstitial(const std::string& InterstitialID) const;
    void initFBAdsNativeAds(const std::string& NativeID, const std::string& layout) const;
    void initFBAdsBanner(const std::string& BannerID) const;
    
    void cacheRewardedAd(const std::string& adsID) const;
    void cacheInterstitialAd(const std::string& adsID) const;
    
    void showBannerAd(const std::string& adsID, int pos) const;
    void hideBannerAd() const;
    
    bool hasInterstitialAd() const;
    bool hasRewardedAd() const;
    
    void showInterstitialAd() const;
    void showRewardedAd() const;
    
    void hideNativeAd(const std::string& adsID);    
    void showNativeAd(const std::string& adsID, int width,
                                      int height, int x, int y);
    
    inline void setCallback(const AdsCallback& cb) { _callback = cb; }
    inline AdsCallback getCallback() { return _callback; }
private:
    AdsCallback _callback;
    std::string onAdsCallback(const std::string& msg) const;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
