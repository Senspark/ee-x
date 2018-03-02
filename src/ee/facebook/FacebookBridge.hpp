//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_
#define EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_

#include <functional>
#include <string>

namespace ee {
namespace facebook {
using ShareResultCallback = std::function<void(bool result)>;

class Facebook final {
public:
    Facebook();
    ~Facebook();

    void shareLinkContent(const std::string& url);
    void sharePhotoContent(const std::string& name);
    void shareVideoContent(const std::string& name);
    void setShareResultCallback(const ShareResultCallback& callback);

protected:
    void onShareResult(bool result);

private:
    ShareResultCallback shareResultCallback_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_ */
