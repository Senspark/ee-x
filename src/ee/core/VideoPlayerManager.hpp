//
//  VideoPlayerManager.hpp
//  ee_x_mobile_core
//
//  Created by eps on 1/31/18.
//

#ifndef EE_X_VIDEO_PLAYER_MANAGER_HPP
#define EE_X_VIDEO_PLAYER_MANAGER_HPP

#include <map>
#include <memory>

namespace ee {
namespace core {
class VideoPlayer;

class VideoPlayerManager {
private:
    using Self = VideoPlayerManager;

public:
    static Self& getInstance();

    /// Creates a video player.
    /// Must be called on the main thread.
    VideoPlayer* createVideoPlayer();

    /// Destroys the specified video player.
    /// Must be called on the main thread.
    bool destroyVideoPlayer(VideoPlayer* player);

private:
    std::map<std::string, std::unique_ptr<VideoPlayer>> players_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_VIDEO_PLAYER_MANAGER_HPP */
