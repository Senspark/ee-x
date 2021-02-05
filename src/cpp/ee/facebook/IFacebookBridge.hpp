//
//  IFacebookBridge.hpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/20/18.
//

#ifndef EE_X_I_FACEBOOK_BRIDGE_HPP
#define EE_X_I_FACEBOOK_BRIDGE_HPP

#ifdef __cplusplus

#include <functional>
#include <string>
#include <vector>

#include <ee/core/IPlugin.hpp>

#include "ee/facebook/FacebookFwd.hpp"

namespace ee {
namespace facebook {
enum class ShareType {
    Link,
    Photo,
    Video,
};

struct LoginResult {
    bool successful;
    bool canceled;
    std::string errorMessage;
};

struct GraphResult {
    bool successful;
    std::string response;
    std::string errorMessage;
};

struct ShareResult {
    bool successful;
    bool canceled;
    std::string errorMessage;
};

class IBridge : public IPlugin {
public:
    virtual bool isLoggedIn() const = 0;

    [[nodiscard]] virtual Task<LoginResult>
    logIn(const std::vector<std::string>& permissions) = 0;

    [[nodiscard]] virtual Task<> logOut() = 0;

    virtual std::shared_ptr<IAccessToken> getAccessToken() const = 0;

    [[nodiscard]] virtual Task<GraphResult>
    graphRequest(const GraphRequest& request) = 0;

    [[nodiscard]] virtual Task<ShareResult>
    shareContent(ShareType type, const std::string& url) = 0;
};
} // namespace facebook
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_FACEBOOK_BRIDGE_HPP */
