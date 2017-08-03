#ifndef EE_X_IronSrc_BRIDGE_HPP_
#define EE_X_IronSrc_BRIDGE_HPP_
#include <memory>
#include <string>
#include <functional>

namespace ee {
namespace ironsrc {
    
enum class IronSrcAdsResultCode
{
    ADS_ERROR,
    ADS_SKIPPED,
    ADS_DID_FINISH
};
    
class IronSrc final{
public:
    IronSrc();
    ~IronSrc();
    
    typedef std::function<void(IronSrcAdsResultCode, const std::string&)> IronSrcAdsCallback;
    
    void initIronSrc(const std::string& gameID);
    
    bool isAdsReady(const std::string& placementID);
    
    void showAds(const std::string& placementID);
    
#pragma mark ====Callback
    IronSrcAdsCallback _callback;
    inline void setCallback(const IronSrcAdsCallback &cb) {
        _callback = cb;
    }
    std::string doCallBack(const std::string& msg) const;
};
} 
} // namespace ee

#endif /* EE_X_IronSrc_BRIDGE_HPP_ */
