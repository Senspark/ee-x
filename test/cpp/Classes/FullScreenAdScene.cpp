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

    handle_ = std::make_shared<ee::ObserverHandle>();
    auto adLoadResult = [](const ee::AdLoadResult& result) {
        CCLOG("network = %s result = %d code = %d message = %s",
              result.network.c_str(), static_cast<int>(result.result),
              result.errorCode, result.errorMessage.c_str());
    };
    (*handle_)
        .bind(*adsManager_->getAppOpenAd())
        .addObserver({
            .onLoadResult = adLoadResult,
        });
    (*handle_)
        .bind(*adsManager_->getInterstitialAd())
        .addObserver({
            .onLoadResult = adLoadResult,
        });
    (*handle_)
        .bind(*adsManager_->getRewardedInterstitialAd())
        .addObserver({
            .onLoadResult = adLoadResult,
        });
    (*handle_)
        .bind(*adsManager_->getRewardedAd())
        .addObserver({
            .onLoadResult = adLoadResult,
        });

    auto resultText = cocos2d::ui::Text::create();
    resultText->setContentSize(cocos2d::Size(250, 80));
    resultText->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2 + 200));
    resultText->setString("---");
    resultText->setFontSize(25);

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

    auto createButton = [winSize](const std::string& title, float deltaY) {
        auto button = cocos2d::ui::Button::create();
        button->setContentSize(cocos2d::Size(250, 80));
        button->setPosition(
            cocos2d::Point(winSize.width / 2, winSize.height / 2 + deltaY));
        button->setTitleText(title);
        button->setTitleFontSize(25);
        return button;
    };

    auto showAppOpenAdButton = createButton("Show App Open Ad", +100);
    showAppOpenAdButton->addClickEventListener(
        std::bind([this, resultText, updateResult] {
            ee::noAwait([this, resultText, updateResult]() -> ee::Task<> {
                resultText->setString("---");
                auto result = co_await adsManager_->getAppOpenAd()->show();
                updateResult(result);
            });
        }));

    auto showInterstitialAdButton = createButton("Show Interstitial Ad", +0);
    showInterstitialAdButton->addClickEventListener(
        std::bind([this, resultText, updateResult] {
            ee::noAwait([this, resultText, updateResult]() -> ee::Task<> {
                resultText->setString("---");
                auto result = co_await adsManager_->getInterstitialAd()->show();
                updateResult(result);
            });
        }));

    auto showRewardedInterstitialAdButton =
        createButton("Show Rewarded Interstitial Ad", -100);
    showRewardedInterstitialAdButton->addClickEventListener(
        std::bind([this, resultText, updateResult] {
            ee::noAwait([this, resultText, updateResult]() -> ee::Task<> {
                resultText->setString("---");
                auto result =
                    co_await adsManager_->getRewardedInterstitialAd()->show();
                updateResult(result);
            });
        }));

    auto showRewardedAdButton = createButton("Show Rewarded Ad", -200);
    showRewardedAdButton->addClickEventListener(
        std::bind([this, resultText, updateResult] {
            ee::noAwait([this, resultText, updateResult]() -> ee::Task<> {
                resultText->setString("---");
                auto result = co_await adsManager_->getRewardedAd()->show();
                updateResult(result);
            });
        }));

    addChild(resultText);
    addChild(showAppOpenAdButton);
    addChild(showInterstitialAdButton);
    addChild(showRewardedInterstitialAdButton);
    addChild(showRewardedAdButton);
    return true;
}
} // namespace eetest
