//
//  TestScene.hpp
//  ee_x_test
//
//  Created by eps on 1/25/21.
//

#ifndef EE_X_TEST_MAIN_SCENE_HPP
#define EE_X_TEST_MAIN_SCENE_HPP

#include <2d/CCNode.h>

namespace eetest {
class MainScene : public cocos2d::Node {
private:
    using Self = MainScene;
    using Super = cocos2d::Node;

public:
    static cocos2d::Scene* openScene();

private:
    virtual bool init() override;
};
} // namespace eetest

#endif /* EE_X_TEST_MAIN_SCENE_HPP */
