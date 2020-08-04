//
//  AdMob.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "AdMobTester.hpp"

#include <ee/AdMob.hpp>

#include "BannerAdTester.hpp"
#include "InterstitialAdTester.hpp"
#include "NativeAdTester.hpp"
#include "RewardedAdTester.hpp"

namespace eetest {
namespace admob {
using Self = Tester;

void Self::initialize() {
    plugin_ = std::make_shared<ee::AdMob>();
    plugin_->initialize();
    plugin_->addTestDevice(plugin_->getEmulatorTestDeviceHash());
    plugin_->addTestDevice("930A5959F4325BAA45E24449B03CB221"); // BlueStacks
    plugin_->addTestDevice("137E2FB99476DB666A99FC3C9F585D65"); // Nexus 5.
}

void Self::destroy() {
    plugin_->destroy();
}

void Self::start() {
    testers_.push_back(std::make_shared<BannerAdTester>(plugin_->createBannerAd(
        config::banner_ad_id, ee::AdMobBannerAdSize::Normal)));

    testers_.push_back(std::make_shared<NativeAdTester>(
        plugin_->createNativeAd(config::native_ad_id, "admob_native_spin",
                                ee::AdMobNativeAdLayout()
                                    .setBody("ad_body")
                                    .setCallToAction("ad_call_to_action")
                                    .setHeadline("ad_headline")
                                    .setIcon("ad_icon")
                                    .setImage("ad_image")
                                    .setMedia("ad_media")
                                    .setPrice("ad_price")
                                    .setStarRating("ad_star_rating")
                                    .setStore("ad_store"))));

    testers_.push_back(std::make_shared<InterstitialAdTester>(
        plugin_->createInterstitialAd(config::interstitial_ad_id)));

    testers_.push_back(std::make_shared<RewardedAdTester>(
        plugin_->createRewardedAd(config::rewarded_ad_id)));

    for (auto&& tester : testers_) {
        tester->initialize();
        tester->start();
    }
}

void Self::stop() {
    for (auto&& tester : testers_) {
        tester->stop();
        tester->destroy();
    }
    testers_.clear();
}
} // namespace admob
} // namespace eetest
