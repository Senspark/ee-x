//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FACEBOOK_FWD_HPP_
#define EE_X_FACEBOOK_FWD_HPP_

#ifdef __cplusplus

#include <memory>
#include <string>
#include <vector>

#include <ee/CoreFwd.hpp>

namespace ee {
namespace facebook {
class IBridge;
class Bridge;
class IAccessToken;
class AccessToken;
template <std::size_t Id, class... Args>
class IDelegate;
using ILoginDelegate =
    IDelegate<0,                                   // Id
              const std::shared_ptr<IAccessToken>& // accessToken
              >;
using IRequestDelegate =
    IDelegate<1,                              // ID
              const std::string&,             // requestId,
              const std::vector<std::string>& // requestRecipients
              >;
using IShareDelegate = IDelegate<2,                 // Id
                                 const std::string& // postId
                                 >;
using IGraphDelegate = IDelegate<3,                 // Id
                                 const std::string& // content
                                 >;
class LoginDelegate;
class ShareDelegate;
class RequestDelegate;
class RequestContent;
class GraphRequest;
} // namespace facebook

using Facebook = facebook::Bridge;
using IFacebook = facebook::IBridge;
using IFacebookAccessToken = facebook::IAccessToken;
using IFacebookLoginDelegate = facebook::ILoginDelegate;
using IFacebookRequestDelegate = facebook::IRequestDelegate;
using IFacebookGraphDelegate = facebook::IGraphDelegate;
using IFacebookShareDelegate = facebook::IShareDelegate;
using FacebookRequestContent = facebook::RequestContent;
using FacebookGraphRequest = facebook::GraphRequest;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FACEBOOK_FWD_HPP_ */
