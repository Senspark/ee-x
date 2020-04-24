//
//  IFacebookBridge.hpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/20/18.
//

#ifndef EE_X_I_FACEBOOK_BRIDGE_HPP
#define EE_X_I_FACEBOOK_BRIDGE_HPP

#include <functional>
#include <string>
#include <vector>

#include "ee/FacebookFwd.hpp"

namespace ee {
namespace facebook {
class IBridge {
public:
    virtual ~IBridge() = default;

    virtual void destroy() = 0;

    virtual bool isLoggedIn() const = 0;

    virtual void logIn(const std::vector<std::string>& permissions,
                       const std::shared_ptr<ILoginDelegate>& delegate) = 0;

    virtual std::shared_ptr<ILoginDelegate> createLoginDelegate() = 0;

    virtual void logOut() = 0;

    virtual std::shared_ptr<IAccessToken> getAccessToken() const = 0;

    virtual void
    graphRequest(const GraphRequest& request,
                 const std::shared_ptr<IGraphDelegate>& delegate) = 0;

    virtual std::shared_ptr<IGraphDelegate> createGraphDelegate() = 0;

    virtual void
    sendRequest(const RequestContent& content,
                const std::shared_ptr<IRequestDelegate>& delegate) = 0;

    virtual std::shared_ptr<IRequestDelegate> createRequestDelegate() = 0;

    virtual void
    shareLinkContent(const std::string& url,
                     const std::shared_ptr<IShareDelegate>& delegate) = 0;

    virtual void
    sharePhotoContent(const std::string& url,
                      const std::shared_ptr<IShareDelegate>& delegate) = 0;

    virtual void
    shareVideoContent(const std::string& url,
                      const std::shared_ptr<IShareDelegate>& delegate) = 0;

    virtual std::shared_ptr<IShareDelegate> createShareDelegate() = 0;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_I_FACEBOOK_BRIDGE_HPP */
