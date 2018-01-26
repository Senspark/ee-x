//
//  Created by Zinge on 7/4/16.
//
//

#include "ee/facebooksocial/FacebookBridge.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
namespace {
// clang-format off
constexpr auto k__facebook_share_link_content           = "__facebook_share_link_content";
constexpr auto k__facebook_share_photo_content          = "__facebook_share_photo_content";
constexpr auto k__facebook_share_video_content          = "__facebook_share_video_content";
constexpr auto k__facebook_get_video_record_url         = "__facebook_get_video_record_url";
constexpr auto k__facebook_record_screen                = "__facebook_record_screen";
constexpr auto k__facebook_stop_record_screen           = "__facebook_stop_record_screen";
    
constexpr auto k__facebook_cancel_record_screen         = "__facebook_cancel_record_screen";
constexpr auto k__facebook_check_permission_record      = "__facebook_check_permission_record";
    
constexpr auto k__onResult                              = "__facebook_share_onResult";
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
    
void Facebook::shareVideoContent(const std::string &name) const
{
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__facebook_share_video_content, name);
}
    
std::string Facebook::getVideoRecordUrl() const
{
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__facebook_get_video_record_url);
}
    
void Facebook::recordScreen() const
{
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__facebook_record_screen);
}
    
void Facebook::stopRecordScreen() const
{
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__facebook_stop_record_screen);
}
    
void Facebook::cancelRecordScreen() const
{
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__facebook_cancel_record_screen);
}
    
bool Facebook::checkPermissionRecord() const
{    
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__facebook_check_permission_record);
    return core::toBool(response);
}
} // namespace crashlytics
} // namespace ee
