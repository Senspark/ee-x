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

namespace eetest {
NativeAdTestScene* createMultiNativeAdTestScene() {
    auto adView =
        ee::runOnUiThreadAndWaitResult<std::shared_ptr<ee::AdViewInterface>>(
            [] {
                auto ad = std::make_shared<ee::MultiAdView>();
                ad->addItem(createAdMobNativeAd());
                ad->addItem(createFacebookNativeAd());
                ad->setSize(100, 100);
                return ad;
            });
    return NativeAdTestScene::createWithAdView(adView);
}
} // namespace eetest
