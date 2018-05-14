//
//  FacebookAds.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_FACEBOOK_ADS_HPP
#define EE_X_TEST_FACEBOOK_ADS_HPP

#include <ee/FacebookAds.hpp>

namespace eetest {
ee::FacebookAds* getFacebookAds();

std::string getFacebookInterstitialAdId();

std::shared_ptr<ee::IAdView> createFacebookNativeAd();

std::shared_ptr<ee::IInterstitialAd> createFacebookInterstitialAd();

void testFacebookNativeAd();

void testFacebookInterstitialAd();
} // namespace eetest

#endif /* EE_X_TEST_FACEBOOK_ADS_HPP */
