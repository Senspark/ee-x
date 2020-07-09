//
//  FacebookAds.cpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#include "FacebookAdsTester.hpp"

#include <ee/FacebookAds.hpp>

#include "InterstitialAdTester.hpp"
#include "NativeAdTester.hpp"
#include "Utils.hpp"

namespace eetest {
namespace facebook_ads {
using Self = Tester;

void Self::initialize() {
    plugin_ = std::make_shared<ee::FacebookAds>();
    plugin_->clearTestDevices();
    plugin_->addTestDevice(plugin_->getTestDeviceHash());
    plugin_->addTestDevice("ad45c323f6a9b07f7a9c072549efb279"); // BlueStacks.
}

void Self::destroy() {
    plugin_->destroy();
}

void Self::start() {
    testers_.push_back(std::make_shared<NativeAdTester>(plugin_->createNativeAd(
        config::native_ad_id, "fb_native_spin",
        ee::FacebookNativeAdLayout()
            .setAdChoices("ad_choices_container")
            .setBody("native_ad_body")
            .setCallToAction("native_ad_call_to_action")
            .setIcon("native_ad_icon")
            .setMedia("native_ad_media")
            .setSocialContext("native_ad_social_context")
            .setTitle("native_ad_title"))));

    testers_.push_back(std::make_shared<InterstitialAdTester>(
        plugin_->createInterstitialAd(config::interstitial_ad_id)));

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
} // namespace facebook_ads
} // namespace eetest
