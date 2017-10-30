//
//  UnityAds.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_UNITY_ADS_HPP
#define EE_X_TEST_UNITY_ADS_HPP

#include <ee/UnityAds.hpp>

namespace eetest {
ee::UnityAds* getUnityAds();

std::string getUnityGameId();
std::string getUnityInterstitialAdId();
std::string getUnityRewardedVideoId();

void testUnityAdsRewardedVideo();
} // namespace eetest

#endif /* EE_X_TEST_UNITY_ADS_HPP */
