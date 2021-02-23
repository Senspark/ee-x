//
//  NativeAdTester.cpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#include "NativeAdTester.hpp"

#include <base/CCDirector.h>
#include <platform/CCGLView.h>

#include <ee/Cpp.hpp>

#include "Utils.hpp"

namespace eetest {
using Self = NativeAdTester;

Self::NativeAdTester(const std::shared_ptr<ee::IBannerAd>& ad)
    : ad_(ad) {}

void Self::initialize() {
    ee::noAwait(ad_->load());
    ad_->setSize(600, 100);
    ad_->setVisible(true);
}

void Self::destroy() {}

void Self::start() {
    auto&& frameSize =
        cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    auto screenWidth = frameSize.width;
    auto screenHeight = frameSize.height;

    float delay = 0.0f;
    scheduleForever(delay + 1.0f, 4.0f, [this] {
        getLogger().info("Load native ad");
        ee::noAwait(ad_->load());
    });

    scheduleOnce(delay += 1.0f, [this, screenWidth, screenHeight] {
        getLogger().info("Resize = screen size / 4");
        ad_->setPosition(3 * screenWidth / 8, 3 * screenHeight / 8);
        ad_->setSize(screenWidth / 4, screenHeight / 4);
    });

    scheduleOnce(delay += 1.0f, [this] {
        getLogger().info("Move to top-left");
        ad_->setPosition(0, 0);
    });

    scheduleOnce(delay += 1.0f, [this, screenWidth] {
        getLogger().info("Move to top-right");
        auto [width, height] = ad_->getSize();
        ad_->setPosition(screenWidth - width, 0);
    });

    scheduleOnce(delay += 1.0f, [this, screenWidth, screenHeight] {
        getLogger().info("Move to bottom-right");
        auto [width, height] = ad_->getSize();
        ad_->setPosition(screenWidth - width, screenHeight - height);
    });

    scheduleOnce(delay += 1.0f, [this, screenHeight] {
        getLogger().info("Move to bottom-left");
        auto [width, height] = ad_->getSize();
        ad_->setPosition(0, screenHeight - height);
    });

    scheduleOnce(delay += 1.0f, [this, screenWidth, screenHeight] {
        getLogger().info("Move to center");
        auto [width, height] = ad_->getSize();
        ad_->setPosition((screenWidth - width) / 2,
                         (screenHeight - height) / 2);
    });

    scheduleOnce(delay += 1.0f, [this, screenWidth, screenHeight] {
        getLogger().info("Resize = screen size");
        ad_->setPosition(0, 0);
        ad_->setSize(screenWidth, screenHeight);
    });

    scheduleOnce(delay += 1.0f, [this, screenWidth, screenHeight] {
        getLogger().info("Resize = screen size / 2");
        ad_->setPosition(screenWidth / 4, screenHeight / 4);
        ad_->setSize(screenWidth / 2, screenHeight / 2);
    });
}

void Self::stop() {}
} // namespace eetest
