//
//  UnityAds.hpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#ifndef EE_X_UNITY_ADS_BRIDGE_HPP_
#define EE_X_UNITY_ADS_BRIDGE_HPP_
#include <memory>
#include <string>
#include <functional>

namespace ee {
namespace unitylib {
    
enum class UnityAdsResultCode
{
    ADS_ERROR,
    ADS_SKIPPED,
    ADS_DID_FINISH
};
    
class UnityAds final{
public:
    UnityAds();
    ~UnityAds();
    
    typedef std::function<void(UnityAdsResultCode, const std::string&)> AdsCallback;
    
    void initUnityAds(const std::string& gameID);
    
    bool isAdsReady(const std::string& placementID);
    
    void showAds(const std::string& placementID);
    
#pragma mark ====Callback
    AdsCallback _callback;
    inline void setCallback(const AdsCallback &cb) {
        _callback = cb;
    }
    std::string doCallBack(const std::string& msg) const;
};
} 
} // namespace ee

#endif /* EE_X_FACEBOOK_ADS_BRIDGE_HPP_ */
