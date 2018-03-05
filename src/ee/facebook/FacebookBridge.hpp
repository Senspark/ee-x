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
using ShareCallback = std::function<void(bool result)>;
using LoginCallback = std::function<void(bool result)>;

class Facebook final {
public:
    Facebook();
    ~Facebook();

    bool isLoggedIn() const;
    void logIn(const std::vector<std::string>& permissions);
    void logOut();
    std::string getAccessToken() const;
    std::string getUserId() const;
    void setLoginCallback(const LoginCallback& callback);
    
    void shareLinkContent(const std::string& url);
    void sharePhotoContent(const std::string& name);
    void shareVideoContent(const std::string& name);
    void setShareCallback(const ShareCallback& callback);

protected:
    void onLoginResult(bool result);
    void onProfileChanged(const std::string& profile);
    void onShareResult(bool result);

private:
    bool isLoggingIn_;
    LoginCallback loginCallback_;
    ShareCallback shareCallback_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_FACEBOOK_BRIDGE_HPP_ */
