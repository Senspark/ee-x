//
//  FacebookGraphRequest.cpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/22/18.
//

#include "ee/facebook/FacebookGraphRequest.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = GraphRequest;

Self& Self::setPath(const std::string& path) {
    path_ = path;
    return *this;
}

Self& Self::setParameter(const std::string& key, const std::string& value) {
    parameters_[key] = value;
    return *this;
}

std::string Self::toString() const {
    nlohmann::json json;
    json["path"] = path_;
    json["parameters"] = parameters_;
    return json.dump();
}
} // namespace facebook
} // namespace ee
