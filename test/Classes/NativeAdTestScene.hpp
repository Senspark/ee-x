//
//  NativeAdTestScene.hpp
//  ee_x_test
//
//  Created by Zinge on 11/6/17.
//
//

#ifndef EE_X_TEST_NATIVE_ADS_TEST_SCENE_HPP
#define EE_X_TEST_NATIVE_ADS_TEST_SCENE_HPP

#include <ee/AdsFwd.hpp>

#include <cocos2d.h>

namespace eetest {
class NativeAdTestScene : public cocos2d::Scene {
private:
    using Self = NativeAdTestScene;
    using Super = cocos2d::Scene;

public:
    static Self* createWithAdView(std::shared_ptr<ee::IAdView> adView);

protected:
    NativeAdTestScene();

    bool initWithAdView(std::shared_ptr<ee::IAdView> adView);

    virtual void onEnter() override;
    virtual void onExit() override;

private:
    std::shared_ptr<ee::IAdView> adView_;

    bool firstTouch_;
    cocos2d::Point lastPosition_;
};
} // namespace eetest

#endif /* EE_X_TEST_NATIVE_ADS_TEST_SCENE_HPP */
