//
//  VideoPlayerTestScene.hpp
//  ee_x_test
//
//  Created by eps on 1/31/18.
//

#ifndef EE_X_VIDEO_PLAYER_TEST_SCENE_HPP
#define EE_X_VIDEO_PLAYER_TEST_SCENE_HPP

#include <ee/CoreFwd.hpp>

#include <cocos2d.h>

namespace eetest {
class VideoPlayerTestScene : public cocos2d::Scene {
private:
    using Self = VideoPlayerTestScene;
    using Super = cocos2d::Scene;

public:
    static Self* create();

protected:
    VideoPlayerTestScene();

    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

private:
    ee::VideoPlayer* player_;
};
} // namespace eetest

#endif /* EE_X_VIDEO_PLAYER_TEST_SCENE_HPP */
