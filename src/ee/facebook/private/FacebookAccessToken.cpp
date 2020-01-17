
//
//  FacebookAccessToken.cpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/21/18.
//

#include "ee/facebook/private/FacebookAccessToken.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = AccessToken;

Self::AccessToken(const std::string& json_) {
    auto json = nlohmann::json::parse(json_);
    token_ = json["token"];
    applicationId_ = json["applicationId"];
    userId_ = json["userId"];
}

std::string Self::getToken() const {
    return token_;
}

std::string Self::getApplicationId() const {
    return applicationId_;
}

std::string Self::getUserId() const {
    return userId_;
}
} // namespace facebook
} // namespace ee
