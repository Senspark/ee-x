//
//  IFacebookAccessToken.hpp-.h
//  ee_x_mobile_facebook
//
//  Created by eps on 3/21/18.
//

#ifndef EE_X_I_FACEBOOK_ACCESS_TOKEN_HPP
#define EE_X_I_FACEBOOK_ACCESS_TOKEN_HPP

#ifdef __cplusplus

#include <string>

#include "ee/FacebookFwd.hpp"

namespace ee {
namespace facebook {
class IAccessToken {
public:
    virtual ~IAccessToken() = default;

    /// Gets the string representing the access token.
    virtual std::string getToken() const = 0;

    /// Gets the ID of the Facebook application associated with this access
    /// token.
    virtual std::string getApplicationId() const = 0;

    /// Returns the user ID for this access token.
    virtual std::string getUserId() const = 0;
};
} // namespace facebook
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_FACEBOOK_ACCESS_TOKEN_HPP */
