//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FACEBOOK_FWD_HPP
#define EE_X_FACEBOOK_FWD_HPP

#ifdef __cplusplus

#include <memory>
#include <string>
#include <vector>

#include <ee/core/CoreFwd.hpp>

namespace ee {
namespace facebook {
enum class ShareType;
struct LoginResult;
struct GraphResult;
struct ShareResult;
class IBridge;
class Bridge;
class IAccessToken;
class AccessToken;
class RequestContent;
class GraphRequest;
} // namespace facebook

using Facebook = facebook::Bridge;
using IFacebook = facebook::IBridge;
using IFacebookAccessToken = facebook::IAccessToken;
using FacebookShareType = facebook::ShareType;
using FacebookLoginResult = facebook::LoginResult;
using FacebookGraphResult = facebook::GraphResult;
using FacebookShareResult = facebook::ShareResult;
using FacebookRequestContent = facebook::RequestContent;
using FacebookGraphRequest = facebook::GraphRequest;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FACEBOOK_FWD_HPP */
