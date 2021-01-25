//
//  TestBannerAdScene.hpp
//  ee_x_test
//
//  Created by eps on 1/25/21.
//

#ifndef EE_X_TEST_BANNER_AD_SCENE_HPP
#define EE_X_TEST_BANNER_AD_SCENE_HPP

#include <2d/CCNode.h>

#include <ee/CppFwd.hpp>

namespace eetest {
class BannerAdScene : public cocos2d::Node {
private:
    using Self = BannerAdScene;
    using Super = cocos2d::Node;

public:
    static cocos2d::Scene*
    openScene(const std::shared_ptr<ee::IAdsManager>& adsManager);

private:
    virtual bool init() override;

    std::shared_ptr<ee::IAdsManager> adsManager_;
};
} // namespace eetest

#endif /* EE_X_TEST_BANNER_AD_SCENE_HPP */
