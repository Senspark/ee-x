//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/facebook/FacebookBridge.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
namespace {
// clang-format off
constexpr auto k__shareLinkContent  = "Facebook_shareLinkContent";
constexpr auto k__sharePhotoContent = "Facebook_sharePhotoContent";
constexpr auto k__shareVideoContent = "Facebook_shareVideoContent";
constexpr auto k__onShareResult     = "Facebook_onShareResult";
// clang-format on
} // namespace

using Self = Facebook;

Self::Facebook() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            auto result = core::toBool(message);
            onShareResult(result);
            return "";
        },
        k__onShareResult);
}

Self::~Facebook() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.deregisterHandler(k__onShareResult);
}

void Self::onShareResult(bool result) {
    if (shareResultCallback_) {
        shareResultCallback_(result);
    }
}

void Self::shareLinkContent(const std::string& url) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__shareLinkContent, url);
}

void Facebook::sharePhotoContent(const std::string& name) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__sharePhotoContent, name);
}

void Facebook::shareVideoContent(const std::string& name) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__shareVideoContent, name);
}
} // namespace facebook
} // namespace ee
