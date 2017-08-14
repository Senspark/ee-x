//
//  UnityAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_UNITY_ADS_BRIDGE_HPP_
#define EE_X_UNITY_ADS_BRIDGE_HPP_

#include <functional>
#include <memory>
#include <string>

namespace ee {
namespace unityads {
enum class UnityAdsResultCode { ADS_ERROR, ADS_SKIPPED, ADS_DID_FINISH };

class UnityAds final {
public:
    using AdCallback = std::function<void(UnityAdsResultCode code,
                                          const std::string& message)>;

    UnityAds();
    ~UnityAds();

    void initUnityAds(const std::string& gameId);

    bool isAdsReady(const std::string& placementId);

    void showAds(const std::string& placementId);

    void setCallback(const AdCallback& callback) { callback_ = callback; }

public:
    std::string doCallBack(const std::string& msg) const;

    AdCallback callback_;
};
} // namespace unityads
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
