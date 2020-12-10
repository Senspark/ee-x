//
//  FacebookRequest.cpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/21/18.
//

#include "ee/facebook/FacebookRequestContent.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = RequestContent;

Self::RequestContent() {
    setActionType(ActionType::None);
    setFilter(Filter::None);
}

Self& Self::setActionType(ActionType type) {
    actionType_ = type;
    return *this;
}

Self& Self::setFilter(Filter filter) {
    filter_ = filter;
    return *this;
}

Self& Self::setRecipients(const std::vector<std::string>& recipients) {
    recipients_ = recipients;
    return *this;
}

Self& Self::setObjectId(const std::string& objectId) {
    objectId_ = objectId;
    return *this;
}

Self& Self::setTitle(const std::string& title) {
    title_ = title;
    return *this;
}

Self& Self::setMessage(const std::string& message) {
    message_ = message;
    return *this;
}

Self& Self::setData(const std::string& data) {
    data_ = data;
    return *this;
}

std::string Self::toString() const {
    nlohmann::json json;
    json["actionType"] = static_cast<int>(actionType_);
    json["filter"] = static_cast<int>(filter_);
    if (not recipients_.empty()) {
        json["recipients"] = recipients_;
    }
    if (not objectId_.empty()) {
        json["objectId"] = objectId_;
    }
    if (not title_.empty()) {
        json["title"] = title_;
    }
    json["message"] = message_;
    if (not data_.empty()) {
        json["data"] = data_;
    }
    return json.dump();
}
} // namespace facebook
} // namespace ee
