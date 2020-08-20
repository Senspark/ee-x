//
//  FacebookAds.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_FACEBOOK_ADS_TESTER_HPP
#define EE_X_TEST_FACEBOOK_ADS_TESTER_HPP

#include <memory>
#include <vector>

#include <ee/FacebookAdsFwd.hpp>

#include "ITester.hpp"

namespace eetest {
namespace facebook_ads {
namespace config {
// clang-format off
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto native_ad_id       = "869337403086643_1444948412192203";
constexpr auto interstitial_ad_id = "869337403086643_1447442535276124";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto native_ad_id       = "869337403086643_1444948412192203";
constexpr auto interstitial_ad_id = "869337403086643_1447441308609580";
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
    std::shared_ptr<ee::IFacebookAds> plugin_;
    std::vector<std::shared_ptr<ITester>> testers_;
};
} // namespace facebook_ads

using FacebookAdsTester = facebook_ads::Tester;
} // namespace eetest

#endif /* EE_X_TEST_FACEBOOK_ADS_TESTER_HPP */
