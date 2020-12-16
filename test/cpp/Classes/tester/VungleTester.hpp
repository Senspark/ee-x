//
//  Vungle.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_VUNGLE_TESTER_HPP
#define EE_X_TEST_VUNGLE_TESTER_HPP

#include <memory>
#include <vector>

#include <ee/CppFwd.hpp>

#include "ITester.hpp"

namespace eetest {
namespace vungle {
namespace config {
// clang-format off
constexpr auto rewarded_ad_id = "rewarded";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto app_id         = "com.senspark.goldminerclassic";
#else  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
constexpr auto app_id         = "651916412";
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
    std::shared_ptr<ee::IVungle> plugin_;
    std::vector<std::shared_ptr<ITester>> testers_;
};
} // namespace vungle

using VungleTester = vungle::Tester;
} // namespace eetest

#endif /* EE_X_TEST_VUNGLE_HPP */
