//
//  AdMob.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "AdMob.hpp"
#include "Utils.hpp"

#include <ee/Core.hpp>

#include <base/CCDirector.h>
#include <platform/CCGLView.h>

namespace eetest {
ee::AdMob* getAdMob() {
    static auto plugin = ee::AdMob();
    static bool initialized = false;
    if (not initialized) {
        ee::runOnUiThreadAndWait([] {
            //
        });
        initialized = true;
    }
    return &plugin;
}

void testAdMobBannerAd() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int screenWidth = static_cast<int>(frameSize.width);
    int screenHeight = static_cast<int>(frameSize.height);

    std::shared_ptr<ee::AdViewInterface> bannerAd;

    ee::runOnUiThreadAndWait([&bannerAd] {
        getLogger().info("Create AdMob banner ad begin");
        bannerAd =
            getAdMob()->createBannerAd("ca-app-pub-3940256099942544/6300978111",
                                       ee::AdMobBannerAdSize::Normal);
        bannerAd->load();
        bannerAd->setVisible(false);
        getLogger().info("Create AdMob banner ad end");
    });

    float delay = 0.0f;
    scheduleOnce(delay += 2.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("show");
            bannerAd->setVisible(true);

            int width, height;
            std::tie(width, height) = bannerAd->getSize();
            bannerAd->setPosition((screenWidth - width) / 2,
                                  (screenHeight - height) / 2);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("Move to top-left");
            bannerAd->setPosition(0, 0);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("Move to top-right");
            int width, height;
            std::tie(width, height) = bannerAd->getSize();
            bannerAd->setPosition(screenWidth - width, 0);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("Move to bottom-right");
            int width, height;
            std::tie(width, height) = bannerAd->getSize();
            bannerAd->setPosition(screenWidth - width, screenHeight - height);
        });
    });

    scheduleForever(delay += 2.0f, 8.0f, [screenWidth, screenHeight, bannerAd] {
        ee::runOnUiThread([screenWidth, screenHeight, bannerAd] {
            getLogger().info("Move to bottom-left");
            int width, height;
            std::tie(width, height) = bannerAd->getSize();
            bannerAd->setPosition(0, screenHeight - height);
        });
    });
}

void testAdMobInterstitial() {
    std::shared_ptr<ee::InterstitialAdInterface> interstitialAd;

    ee::runOnUiThreadAndWait([&interstitialAd] {
        getLogger().info("Create AdMob interstitial ad begin");
        interstitialAd = getAdMob()->createInterstitialAd(
            "ca-app-pub-3940256099942544/1033173712");
        getLogger().info("Create AdMob interstitial ad end");
    });

    scheduleForever(1.0f, 3.0f, [interstitialAd] {
        ee::runOnUiThread([interstitialAd] {
            getLogger().info("Load AdMob interstitial ad");
            interstitialAd->load();
        });
    });
    scheduleForever(2.0f, 3.0f, [interstitialAd] {
        ee::runOnUiThread([interstitialAd] {
            getLogger().info("Show AdMob interstitial ad");
            interstitialAd->show();
        });
    });
}
} // namespace eetest
