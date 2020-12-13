//
//  AdMob.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_ADMOB_TESTER_HPP
#define EE_X_TEST_ADMOB_TESTER_HPP

#include <memory>
#include <vector>

#include <ee/CppFwd.hpp>

#include "ITester.hpp"

namespace eetest {
namespace admob {
namespace config {
// clang-format off
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto app_id                   = "ca-app-pub-3940256099942544~3347511713";
constexpr auto banner_ad_id             = "ca-app-pub-3940256099942544/6300978111";
constexpr auto native_ad_id             = "ca-app-pub-3940256099942544/2247696110";
constexpr auto native_video_ad_id       = "ca-app-pub-3940256099942544/1044960115";
constexpr auto interstitial_ad_id       = "ca-app-pub-3940256099942544/1033173712";
constexpr auto interstitial_video_ad_id = "ca-app-pub-3940256099942544/8691691433";
constexpr auto rewarded_ad_id           = "ca-app-pub-3940256099942544/5224354917";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto app_id                   = "ca-app-pub-3940256099942544~1458002511";
constexpr auto banner_ad_id             = "ca-app-pub-3940256099942544/2934735716";
constexpr auto native_ad_id             = "ca-app-pub-3940256099942544/3986624511";
constexpr auto native_video_ad_id       = "ca-app-pub-3940256099942544/2521693316";
constexpr auto interstitial_ad_id       = "ca-app-pub-3940256099942544/4411468910";
constexpr auto interstitial_video_ad_id = "ca-app-pub-3940256099942544/5135589807";
constexpr auto rewarded_ad_id           = "ca-app-pub-3940256099942544/1712485313";
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
    std::shared_ptr<ee::IAdMob> plugin_;
    std::vector<std::shared_ptr<ITester>> testers_;
};
} // namespace admob

using AdMobTester = admob::Tester;
} // namespace eetest

#endif /* EE_X_TEST_ADMOB_TESTER_HPP */
