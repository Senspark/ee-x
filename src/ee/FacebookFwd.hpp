//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FACEBOOK_FWD_HPP_
#define EE_X_FACEBOOK_FWD_HPP_

#include <memory>
#include <string>
#include <vector>

namespace ee {
namespace facebook {
class IBridge;
class Bridge;
class IAccessToken;
class AccessToken;
template <class... Args>
class IDelegate;
using ILoginDelegate =
    IDelegate<const std::shared_ptr<IAccessToken>& // accessToken
              >;
using IRequestDelegate =
    IDelegate<const std::string&,             // requestId,
              const std::vector<std::string>& // requestRecipients
              >;
using IShareDelegate = IDelegate<const std::string& // postId
                                 >;
using IGraphDelegate = IDelegate<const std::string& // content
                                 >;
class LoginDelegate;
class ShareDelegate;
class RequestDelegate;
class RequestContent;
class GraphRequest;
} // namespace facebook

using IFacebook = facebook::IBridge;
using IFacebookAccessToken = facebook::IAccessToken;
using IFacebookLoginDelegate = facebook::ILoginDelegate;
using IFacebookRequestDelegate = facebook::IRequestDelegate;
using IFacebookShareDelegate = facebook::IShareDelegate;
using FacebookRequestContent = facebook::RequestContent;
using FacebookGraphRequest = facebook::GraphRequest;
} // namespace ee

#endif /* EE_X_FACEBOOK_FWD_HPP_ */
