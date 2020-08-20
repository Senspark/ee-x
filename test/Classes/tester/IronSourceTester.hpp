//
//  IronSource.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_IRON_SOURCE_TESTER_HPP
#define EE_X_TEST_IRON_SOURCE_TESTER_HPP

#include <memory>
#include <vector>

#include <ee/IronSourceFwd.hpp>

#include "ITester.hpp"

namespace eetest {
namespace iron_source {
namespace config {
constexpr auto rewarded_ad_id = "DefaultRewardedVideo";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto app_id = "67a6443d";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto app_id = "67a60ab5";
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
} // namespace config
class Tester : public ITester {
public:
    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
    std::shared_ptr<ee::IIronSource> plugin_;
    std::vector<std::shared_ptr<ITester>> testers_;
};
} // namespace iron_source

using IronSourceTester = iron_source::Tester;
} // namespace eetest

#endif /* EE_X_TEST_IRON_SOURCE_TESTER_HPP */
