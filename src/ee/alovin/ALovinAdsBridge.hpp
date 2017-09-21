//
//  ALovinAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_ALOVIN_ADS_BRIDGE_HPP_
#define EE_X_ALOVIN_ADS_BRIDGE_HPP_

#include <functional>
#include <memory>
#include <string>

namespace ee {
namespace alovinads {
enum class ALovinAdsResultCode { ADS_ERROR, ADS_SKIPPED, ADS_DID_FINISH };

class ALovinAds final {
public:
    using AdCallback = std::function<void(ALovinAdsResultCode code,
                                          const std::string& message)>;

    ALovinAds();
    ~ALovinAds();

    void initALovinAds();

    bool isInterstitialReady();

    void showInterstitial();
    
    bool isRewardVideoReady();
    
    void showRewardVideo();

    void setCallback(const AdCallback& callback) { callback_ = callback; }

public:
    std::string doCallBack(const std::string& msg) const;

    AdCallback callback_;
};
} // namespace alovinads
} // namespace ee

#endif /* EE_X_ALOVIN_ADS_BRIDGE_HPP_ */
