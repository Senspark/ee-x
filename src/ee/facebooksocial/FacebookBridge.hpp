//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_
#define EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "ee/core/LogLevel.hpp"

namespace ee {
namespace facebook {
using ShareResultCallback = std::function<void(bool result)>;    
    
class Facebook final {
    void onResult(const std::string& message);
public:
    Facebook();
    ~Facebook();
    
    void shareLinkContent(const std::string &url) const ;
    void sharePhotoContent(const std::string &name) const ;
    void shareVideoContent(const std::string &name) const ;
    std::string getVideoRecordUrl() const;
    
    void recordScreen() const ;
    void stopRecordScreen() const ;
    void cancelRecordScreen() const ;
    bool checkPermissionRecord() const;
    
    
    void setResultCallback(const ShareResultCallback& callback) { callback_ = callback; }
private:
    ShareResultCallback callback_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_ */
