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
class FacebookBannerAd;
class FacebookNativeAd;

enum class FacebookBannerAdSize {

};

class FacebookAds final {
public:
    using AdCallback = std::function<void(const std::string& message)>;

    FacebookAds();
    ~FacebookAds();

    std::shared_ptr<FacebookBannerAd>
    createBannerAd(const std::string& adId, FacebookBannerAdSize adSize);

    bool destroyBannerAd(const std::string& adId);

    std::shared_ptr<FacebookNativeAd> createNativeAd(const std::string& adId,
                                                     const std::string& layout);

    bool destroyNativead(const std::string& adId);

    void cacheInterstitialAd(const std::string& adId) const;
    bool showInterstitialAd() const;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
