//
//  FacebookShareDelegate.cpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/20/18.
//

#include "ee/facebook/internal/FacebookShareDelegate.hpp"
#include "ee/core/IMessageBridge.hpp"

namespace ee {
namespace facebook {
using Self = ShareDelegate;

namespace {
std::string k__onSuccess(int tag) {
    return "FacebookShareDelegate_onSuccess_" + std::to_string(tag);
}

std::string k__onFailure(int tag) {
    return "FacebookShareDelegate_onFailure_" + std::to_string(tag);
}

std::string k__onCancel(int tag) {
    return "FacebookShareDelegate_onCancel_" + std::to_string(tag);
}
} // namespace

Self::ShareDelegate(IMessageBridge& bridge, int tag)
    : bridge_(bridge)
    , tag_(tag) {
    bridge_.registerHandler(
        [this](const std::string& message) {
            if (successCallback_) {
                successCallback_(message);
            }
            self_.reset();
            return "";
        },
        k__onSuccess(tag_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            if (failureCallback_) {
                failureCallback_(message);
            }
            self_.reset();
            return "";
        },
        k__onFailure(tag_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            if (cancelCallback_) {
                cancelCallback_();
            }
            self_.reset();
            return "";
        },
        k__onCancel(tag_));
}

Self::~ShareDelegate() {
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
