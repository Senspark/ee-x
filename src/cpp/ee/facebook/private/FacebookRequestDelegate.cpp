//
//  FacebookRequestDelegate.cpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/21/18.
//

#include "ee/facebook/private/FacebookRequestDelegate.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Thread.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = RequestDelegate;

namespace {
std::string k__onSuccess(int tag) {
    return "FacebookRequestDelegate_onSuccess_" + std::to_string(tag);
}

std::string k__onFailure(int tag) {
    return "FacebookRequestDelegate_onFailure_" + std::to_string(tag);
}

std::string k__onCancel(int tag) {
    return "FacebookRequestDelegate_onCancel_" + std::to_string(tag);
}
} // namespace

Self::RequestDelegate(IMessageBridge& bridge, int tag)
    : bridge_(bridge)
    , tag_(tag) {
    bridge_.registerHandler(
        [this](const std::string& message) {
            if (successCallback_) {
                auto json = nlohmann::json::parse(message);
                auto&& requestId =
                    json.value("requestId", std::string()); // Null iOS.
                auto&& requestRecipients =
                    json.value("requestRecipients",
                               std::vector<std::string>()); // Null iOS.
                successCallback_(requestId, requestRecipients);
            }
            self_.reset();
        },
        k__onSuccess(tag_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            if (failureCallback_) {
                failureCallback_(message);
            }
            self_.reset();
        },
        k__onFailure(tag_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            if (cancelCallback_) {
                cancelCallback_();
            }
            self_.reset();
        },
        k__onCancel(tag_));
}

Self::~RequestDelegate() {
    bridge_.deregisterHandler(k__onSuccess(tag_));
    bridge_.deregisterHandler(k__onFailure(tag_));
    bridge_.deregisterHandler(k__onCancel(tag_));
}

Self& Self::onSuccess(const SuccessCallback& callback) {
    successCallback_ = callback;
    return *this;
}

Self& Self::onFailure(const FailureCallback& callback) {
    failureCallback_ = callback;
    return *this;
}

Self& Self::onCancel(const CancelCallback& callback) {
    cancelCallback_ = callback;
    return *this;
}
} // namespace facebook
} // namespace ee
