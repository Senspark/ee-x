//
//  InterstitialAdScene.cpp
//  ee_x_test
//
//  Created by eps on 1/25/21.
//

#include "FullScreenAdScene.hpp"

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <ee/Cpp.hpp>

namespace eetest {
using Self = FullScreenAdScene;

cocos2d::Scene*
Self::openScene(const std::shared_ptr<ee::IAdsManager>& adsManager) {
    auto scene = cocos2d::Scene::create();
    auto node = new Self();
    node->autorelease();
    node->adsManager_ = adsManager;
    node->init();
    scene->addChild(node);
    return scene;
}

bool Self::init() {
    if (not Super::init()) {
        return false;
    }
    auto&& winSize = _director->getWinSize();
    setContentSize(winSize);

    auto resultText = cocos2d::ui::Text::create();
    resultText->setContentSize(cocos2d::Size(200, 60));
    resultText->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2 + 50));
    resultText->setString("---");

    auto updateResult = [resultText](ee::AdResult result) {
        switch (result) {
        case ee::AdResult::NotInitialized:
            resultText->setString("Not Initialized");
            break;
        case ee::AdResult::NotConfigured:
            resultText->setString("Not Configured");
            break;
        case ee::AdResult::Capped:
            resultText->setString("Capped");
            break;
        case ee::AdResult::NoInternet:
            resultText->setString("No Internet");
            break;
        case ee::AdResult::NotLoaded:
            resultText->setString("Not Loaded");
            break;
        case ee::AdResult::Failed:
            resultText->setString("Failed");
            break;
        case ee::AdResult::Canceled:
            resultText->setString("Canceled");
            break;
        case ee::AdResult::Completed:
            resultText->setString("Completed");
            break;
        }
    };

    auto showInterstitialAdButton = cocos2d::ui::Button::create();
    showInterstitialAdButton->setContentSize(cocos2d::Size(150, 80));
    showInterstitialAdButton->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2));
    showInterstitialAdButton->setTitleText("Show Interstitial Ad");
    showInterstitialAdButton->addClickEventListener(
        std::bind([this, resultText, updateResult] {
            ee::noAwait([this, resultText, updateResult]() -> ee::Task<> {
                resultText->setString("---");
                auto result = co_await adsManager_->showInterstitialAd();
                updateResult(result);
            });
        }));

    auto showRewardedAdButton = cocos2d::ui::Button::create();
    showRewardedAdButton->setContentSize(cocos2d::Size(150, 80));
    showRewardedAdButton->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2 - 50));
    showRewardedAdButton->setTitleText("Show Rewarded Ad");
    showRewardedAdButton->addClickEventListener(
        std::bind([this, resultText, updateResult] {
            ee::noAwait([this, resultText, updateResult]() -> ee::Task<> {
                resultText->setString("---");
                auto result = co_await adsManager_->showRewardedAd();
                updateResult(result);
            });
        }));

    addChild(resultText);
    addChild(showInterstitialAdButton);
    addChild(showRewardedAdButton);
    return true;
}
} // namespace eetest
