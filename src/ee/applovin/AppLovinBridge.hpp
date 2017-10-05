//
//  ALovinAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_APP_LOVIN_BRIDGE_HPP
#define EE_X_APP_LOVIN_BRIDGE_HPP

#include <functional>
#include <string>

namespace ee {
namespace applovin {
enum class ALovinAdsResultCode { ADS_ERROR, ADS_SKIPPED, ADS_DID_FINISH };

class AppLovin final {
public:
    using AdCallback = std::function<void(ALovinAdsResultCode code,
                                          const std::string& message)>;

    AppLovin();
    ~AppLovin();

    void initialize();

    bool isInterstitialReady();

    bool showInterstitial();

    bool isRewardVideoReady();

    bool showRewardVideo();

    void setCallback(const AdCallback& callback) { callback_ = callback; }

public:
    std::string doCallBack(const std::string& msg) const;

    AdCallback callback_;
};
} // namespace applovin
} // namespace ee

#endif /* EE_X_APP_LOVIN_BRIDGE_HPP */
