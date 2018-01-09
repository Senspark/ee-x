//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/facebooksocial/FacebookBridge.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/MessageBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
namespace {
// clang-format off
constexpr auto k__facebook_share_link_content          = "__facebook_share_link_content";
constexpr auto k__facebook_share_photo_content          = "__facebook_share_photo_content";
    
constexpr auto k__onResult = "__onResult";
// clang-format on
} // namespace

Facebook::Facebook() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onResult(message);
            return "";
        },
        k__onResult);
}
    
Facebook::~Facebook()
{
    auto&& bridge = MessageBridge::getInstance();
    bridge.deregisterHandler(k__onResult);
}

void Facebook::onResult(const std::string& message)
{    
    bool value = (message == "true");
    if (callback_) {
        callback_(value);
    }
}

void Facebook::shareLinkContent(const std::string& url) const {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__facebook_share_link_content, url);
}

void Facebook::sharePhotoContent(const std::string& name) const {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__facebook_share_photo_content, name);
}
} // namespace crashlytics
} // namespace ee
