//
//  VideoPlayerTestScene.cpp
//  ee_x_test
//
//  Created by eps on 1/31/18.
//

#include "VideoPlayerTestScene.hpp"

#include <ee/Cpp.hpp>

namespace eetest {
using Self = VideoPlayerTestScene;

Self* Self::create() {
    auto result = new Self();
    result->init();
    result->autorelease();
    return result;
}

Self::VideoPlayerTestScene()
    : player_(nullptr) {}

bool Self::init() {
    if (not Super::init()) {
        return false;
    }

    auto winSize = _director->getWinSize();
    ee::runOnMainThread([this, winSize] {
        player_ = ee::VideoPlayerManager::getInstance().createVideoPlayer();
        player_->setKeepAspectRatioEnabled(false);
        player_->setVisible(false);
        player_->setPosition(0, 0);
        player_->setSize(
            static_cast<int>(ee::Metrics::fromPoint(winSize.width).toPixel()),
            static_cast<int>(ee::Metrics::fromPoint(winSize.height).toPixel()));
    });
    return true;
}

void Self::onEnter() {
    Super::onEnter();

    auto fileUtils = cocos2d::FileUtils::getInstance();
    auto path = fileUtils->fullPathForFilename("videos/cocosvideo.mp4");
    ee::runOnMainThread([this, path] {
        player_->setVisible(true);
        player_->loadFile(path);
        player_->play();
    });
}

void Self::onExit() {
    Super::onExit();
}
} // namespace eetest
