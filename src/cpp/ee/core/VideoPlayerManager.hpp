//
//  VideoPlayerManager.hpp
//  ee_x_mobile_core
//
//  Created by eps on 1/31/18.
//

#ifndef EE_X_VIDEO_PLAYER_MANAGER_HPP
#define EE_X_VIDEO_PLAYER_MANAGER_HPP

#ifdef __cplusplus

#include <map>
#include <memory>

#include "ee/CoreFwd.hpp"

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
    
protected:
    VideoPlayerManager();

private:
    IMessageBridge& bridge_;
    std::map<std::string, std::unique_ptr<VideoPlayer>> players_;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_VIDEO_PLAYER_MANAGER_HPP */
