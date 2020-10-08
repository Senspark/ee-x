//
//  RewardedAdSceneTester.cpp
//  ee_x_test_mobile
//
//  Created by eps on 10/7/20.
//

#include "RewardedAdSceneTester.hpp"

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <ee/AdMob.hpp>
#include <ee/FacebookAds.hpp>
#include <ee/IronSource.hpp>
#include <ee/UnityAds.hpp>

namespace eetest {
namespace config {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto iron_source_app = "d8d36e59";
constexpr auto unity_ads_app = "3843671";
constexpr auto admob_rewarded_ad = "ca-app-pub-2101587572072038/4784852460";
constexpr auto facebook_ads_rewarded_ad = "1095721240884103_1095725130883714";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto iron_source_app = "d8d2fb49";
constexpr auto unity_ads_app = "3843670";
constexpr auto admob_rewarded_ad = "ca-app-pub-2101587572072038/7027872422";
constexpr auto facebook_ads_rewarded_ad = "1095721240884103_1095724794217081";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto unity_ads_rewarded_ad = "rewardedVideo";
constexpr auto iron_source_rewarded_ad = "DefaultRewardedVideo";
} // namespace config

using Self = RewardedAdSceneTester;

Self::RewardedAdSceneTester()
    : scene_(nullptr) {}

void Self::initialize() {
    adMob_ = ee::PluginManager::createPlugin<ee::IAdMob>();
    facebookAds_ = ee::PluginManager::createPlugin<ee::IFacebookAds>();
    ironSource_ = ee::PluginManager::createPlugin<ee::IIronSource>();
    unityAds_ = ee::PluginManager::createPlugin<ee::IUnityAds>();

    adMob_->initialize();
    ironSource_->initialize(config::iron_source_app);
    unityAds_->initialize(config::unity_ads_app, false);

    handle_ = std::make_unique<ee::ObserverHandle>();

    scene_ = cocos2d::Scene::create();
    scene_->retain();

    auto ad = std::make_shared<ee::MultiRewardedAd>();
    (*ad)
        .addItem(adMob_->createRewardedAd(config::admob_rewarded_ad))
        .addItem(
            facebookAds_->createRewardedAd(config::facebook_ads_rewarded_ad))
        .addItem(ironSource_->createRewardedAd(config::iron_source_rewarded_ad))
        .addItem(unityAds_->createRewardedAd(config::unity_ads_rewarded_ad));
    rewardedAd_ = ad;

    auto winSize = cocos2d::Director::getInstance()->getWinSize();

    auto statusLabel = cocos2d::ui::Text::create();
    statusLabel->setContentSize(cocos2d::Size(200, 60));
    statusLabel->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2 + 100));
    statusLabel->setString("---");

    auto loadButton = cocos2d::ui::Button::create();
    loadButton->setContentSize(cocos2d::Size(150, 80));
    loadButton->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2 + 50));
    loadButton->setTitleText("Load");
    loadButton->addClickEventListener(std::bind([ad, statusLabel] { //
        if (ad->isLoaded()) {
            statusLabel->setString("Loaded");
        } else {
            ee::noAwait(ad->load());
        }
    }));

    auto resultLabel = cocos2d::ui::Text::create();
    resultLabel->setContentSize(cocos2d::Size(200, 60));
    resultLabel->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2 - 0));
    resultLabel->setString("---");

    auto watchButton = cocos2d::ui::Button::create();
    watchButton->setContentSize(cocos2d::Size(150, 80));
    watchButton->setPosition(
        cocos2d::Point(winSize.width / 2, winSize.height / 2 - 50));
    watchButton->setTitleText("Watch");
    watchButton->addClickEventListener(
        std::bind([ad, statusLabel, resultLabel] { //
            ee::noAwait([ad, statusLabel, resultLabel]() -> ee::Task<> {
                statusLabel->setString("---");
                auto result = co_await ad->show();
                switch (result) {
                case ee::IRewardedAdResult::Canceled: {
                    resultLabel->setString("Canceled");
                    break;
                }
                case ee::IRewardedAdResult::Completed: {
                    resultLabel->setString("Completed");
                    break;
                }
                case ee::IRewardedAdResult::Failed: {
                    resultLabel->setString("Failed");
                    break;
                }
                }
                if (ad->isLoaded()) {
                    statusLabel->setString("Loaded");
                }
            });
        }));

    scene_->addChild(statusLabel);
    scene_->addChild(loadButton);
    scene_->addChild(resultLabel);
    scene_->addChild(watchButton);

    handle_->bind(*ad).addObserver({
        .onLoaded =
            [statusLabel] { //
                statusLabel->setString("Loaded");
            },
    });
}

void Self::destroy() {
    scene_->release();
    scene_ = nullptr;
}

void Self::start() {
    cocos2d::Director::getInstance()->pushScene(scene_);
}

void Self::stop() {
    cocos2d::Director::getInstance()->popScene();
}
} // namespace eetest
