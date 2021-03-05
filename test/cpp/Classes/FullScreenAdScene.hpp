//
//  InterstitialAdScene.hpp
//  ee_x_test
//
//  Created by eps on 1/25/21.
//

#ifndef EE_X_TEST_FULL_SCREEN_AD_SCENE_HPP
#define EE_X_TEST_FULL_SCREEN_AD_SCENE_HPP

#include <2d/CCNode.h>

#include <ee/CppFwd.hpp>

namespace eetest {
class FullScreenAdScene : public cocos2d::Node {
private:
    using Self = FullScreenAdScene;
    using Super = cocos2d::Node;

public:
    static cocos2d::Scene*
    openScene(const std::shared_ptr<ee::IAdsManager>& adsManager);

private:
    virtual bool init() override;

    std::shared_ptr<ee::IAdsManager> adsManager_;
    std::shared_ptr<ee::ObserverHandle> handle_;
};
} // namespace eetest

#endif /* EE_X_TEST_FULL_SCREEN_AD_SCENE_HPP */
