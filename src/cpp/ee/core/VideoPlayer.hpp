//
//  VideoPlayer.hpp
//  ee_x_mobile_core
//
//  Created by eps on 1/30/18.
//

#ifndef EE_X_VIDEO_PLAYER_HPP
#define EE_X_VIDEO_PLAYER_HPP

#ifdef __cplusplus

#include <string>

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
class VideoPlayerManager;

class VideoPlayer {
private:
public:
    ~VideoPlayer();

    /// Must be called on the main thread.
    void loadFile(const std::string& path);

    /// Must be called on the main thread.
    void setPosition(int x, int y);

    /// Must be called on the main thread.
    void setSize(int width, int height);

    /// Stars playback.
    /// Must be called on the main thread.
    void play();

    /// Pauses playback.
    /// Must be called on the main thread.
    void pause();

    /// Resumes playback.
    /// Must be called on the main thread.
    void resume();

    /// Stops playback.
    /// Must be called on the main thread.
    void stop();

    bool isVisible() const;

    /// Must be called on the main thread.
    void setVisible(bool visible);

    bool isKeepAspectRatioEnabled() const;

    /// Must be called on the main thread.
    void setKeepAspectRatioEnabled(bool enabled);

    bool isFullScreenEnabled() const;

    /// Must be called on the main thread.
    void setFullScreenEnabled(bool enabled);

protected:
    friend VideoPlayerManager;

    explicit VideoPlayer(IMessageBridge& bridge, const std::string& tag);

private:
    IMessageBridge& bridge_;
    std::string tag_;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_VIDEO_PLAYER_HPP */
