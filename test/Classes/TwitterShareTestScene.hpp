//
//  TwitterShareTestScene.hpp
//  ee_x_test_mobile
//
//  Created by Doan Anh Tai on 10/11/18.
//

#ifndef TwitterShareTestScene_hpp
#define TwitterShareTestScene_hpp

#include <cocos2d.h>
#include <ee/TwitterFwd.hpp>

namespace eetest {
class TwitterShareTestScene : public cocos2d::Scene {
private:
    using ShareResultCallback =
        std::function<void(bool result, bool is_share_twitter)>;
    using Self = TwitterShareTestScene;
    using Super = cocos2d::Scene;

public:
    static Self* create();

protected:
    TwitterShareTestScene();

    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

private:
    std::unique_ptr<ee::Twitter> twitter_;
    ShareResultCallback _callback{nullptr};
    void doShareSuccess(bool result);
};
} // namespace eetest

#endif /* TwitterShareTestScene_hpp */