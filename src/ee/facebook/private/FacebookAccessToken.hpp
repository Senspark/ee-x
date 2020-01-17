//
//  FacebookAccessToken.hpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/21/18.
//

#ifndef EE_X_FACEBOOK_ACCESS_TOKEN_HPP
#define EE_X_FACEBOOK_ACCESS_TOKEN_HPP

#include "ee/facebook/IFacebookAccessToken.hpp"

namespace ee {
namespace facebook {
class AccessToken : public IAccessToken {
public:
    explicit AccessToken(const std::string& json);

    /// @see Super.
    virtual std::string getToken() const override;

    /// @see Super.
    virtual std::string getApplicationId() const override;

    /// @see Super.
    virtual std::string getUserId() const override;

private:
    friend Bridge;
    
    std::string token_;
    std::string applicationId_;
    std::string userId_;
};
} // namespace facebook
} // namespace ee

#endif /* EE_X_FACEBOOK_ACCESS_TOKEN_HPP */
