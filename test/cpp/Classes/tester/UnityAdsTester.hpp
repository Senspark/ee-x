//
//  UnityAds.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_UNITY_ADS_TESTER_HPP
#define EE_X_TEST_UNITY_ADS_TESTER_HPP

#include <memory>
#include <vector>

#include <ee/CppFwd.hpp>

#include "ITester.hpp"

namespace eetest {
namespace unity_ads {
namespace config {
// clang-format off
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto app_id             = "73406";
constexpr auto interstitial_ad_id = "defaultZone";
constexpr auto rewarded_ad_id     = "rewardedVideoZone";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto app_id             = "1423604";
constexpr auto interstitial_ad_id = "video";
constexpr auto rewarded_ad_id     = "rewardedVideo";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
// clang-format on
} // namespace config

class Tester : public ITester {
public:
    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
    std::shared_ptr<ee::IUnityAds> plugin_;
    std::vector<std::shared_ptr<ITester>> testers_;
};
} // namespace unity_ads
} // namespace eetest

#endif /* EE_X_TEST_UNITY_ADS_TESTER_HPP */
