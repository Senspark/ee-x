//
//  MultiNativeAdTestScene.cpp
//  ee_x_test
//
//  Created by Zinge on 11/6/17.
//
//

#include "MultiNativeAdTestScene.hpp"

#include <cocos2d.h>

#include <ee/Ads.hpp>
#include <ee/Core.hpp>

#include "AdMob.hpp"
#include "FacebookAds.hpp"

namespace eetest {
NativeAdTestScene* createMultiNativeAdTestScene() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    auto ad = std::make_shared<ee::MultiAdView>();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS ||                                   \
    CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    ad->addItem(createAdMobNativeAd());
    ad->addItem(createFacebookNativeAd());
#endif
    ad->setSize(screenWidth / 4, screenHeight / 4);
    return NativeAdTestScene::createWithAdView(ad);
}
} // namespace eetest
