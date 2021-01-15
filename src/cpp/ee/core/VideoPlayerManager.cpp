//
//  VideoPlayerManager.cpp
//  ee_x_mobile_core
//
//  Created by eps on 1/31/18.
//

#include "ee/core/VideoPlayerManager.hpp"

#include "ee/core/IMessageBridge.hpp"
#include "ee/core/PluginManager.hpp"
#include "ee/core/VideoPlayer.hpp"

namespace ee {
namespace core {
using Self = VideoPlayerManager;

namespace {
constexpr auto k__create = "VideoPlayerManager_create";
constexpr auto k__destroy = "VideoPlayerManager_destroy";
} // namespace

Self& Self::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

Self::VideoPlayerManager()
    : bridge_(PluginManager::getBridge()) {}

VideoPlayer* Self::createVideoPlayer() {
    static int counter = 0;
    auto tag = std::to_string(counter++);
    bridge_.call(k__create, tag);
    
    auto player = std::unique_ptr<VideoPlayer>(new VideoPlayer(bridge_, tag));
    auto result = player.get();
    players_.emplace(tag, std::move(player));
    return result;
}

bool Self::destroyVideoPlayer(VideoPlayer* player) {
    auto tag = player->tag_;
    auto iter = players_.find(tag);
    if (iter == players_.cend()) {
        return false;
    }
    players_.erase(iter);
    
    bridge_.call(k__destroy, tag);
    return true;
}
} // namespace core
} // namespace ee
