//
//  MultiNativeAdTestScene.cpp
//  ee_x_test
//
//  Created by Zinge on 11/6/17.
//
//

#include "MultiNativeAdTestScene.hpp"
#include "AdMob.hpp"
#include "FacebookAds.hpp"

#include <ee/Ads.hpp>
#include <ee/Core.hpp>

#include <cocos2d.h>

namespace eetest {
NativeAdTestScene* createMultiNativeAdTestScene() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    auto adView =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::AdViewInterface>>(
            [screenWidth, screenHeight] {
                auto ad = std::make_shared<ee::MultiAdView>();
                ad->addItem(createAdMobNativeAd());
                ad->addItem(createFacebookNativeAd());
                ad->setSize(screenWidth / 4, screenHeight / 4);
                return ad;
            });
    return NativeAdTestScene::createWithAdView(adView);
}
} // namespace eetest
