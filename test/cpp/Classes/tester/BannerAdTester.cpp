//
//  BannerAdTester.cpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#include "BannerAdTester.hpp"

#include <base/CCDirector.h>
#include <platform/CCGLView.h>

#include <ee/Ads.hpp>

#include "Utils.hpp"

namespace eetest {
using Self = BannerAdTester;

Self::BannerAdTester(const std::shared_ptr<ee::IAdView>& ad)
    : ad_(ad) {}

void Self::initialize() {
    ee::noAwait(ad_->load());
}

void Self::destroy() {}

void Self::start() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    auto screenWidth = frameSize.width;
    auto screenHeight = frameSize.height;

    float width, height;
    std::tie(width, height) = ad_->getSize();

    float delay = 0.0f;
    scheduleOnce(delay += 2.0f,
                 [this, screenWidth, screenHeight, width, height] {
                     getLogger().info("show");
                     ad_->setVisible(true);
                     ad_->setPosition((screenWidth - width) / 2,
                                      (screenHeight - height) / 2);
                 });

    scheduleForever(delay += 2.0f, 8.0f, [this] {
        getLogger().info("Move to top-left");
        ad_->setPosition(0, 0);
    });

    scheduleForever(delay += 2.0f, 8.0f, [this, screenWidth, width] {
        getLogger().info("Move to top-right");
        ad_->setPosition(screenWidth - width, 0);
    });

    scheduleForever(
        delay += 2.0f, 8.0f, [this, screenWidth, screenHeight, width, height] {
            getLogger().info("Move to bottom-right");
            ad_->setPosition(screenWidth - width, screenHeight - height);
        });

    scheduleForever(delay += 2.0f, 8.0f, [this, screenHeight, height] {
        getLogger().info("Move to bottom-left");
        ad_->setPosition(0, screenHeight - height);
    });
}

void Self::stop() {}
} // namespace eetest
