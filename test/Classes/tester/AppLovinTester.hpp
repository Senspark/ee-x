//
//  AppLovin.hpp
//  ee_x_test
//
//  Created by Zinge on 10/16/17.
//
//

#ifndef EE_X_TEST_APP_LOVIN_TESTER_HPP
#define EE_X_TEST_APP_LOVIN_TESTER_HPP

#include <memory>
#include <vector>

#include <ee/AppLovinFwd.hpp>

#include "ITester.hpp"

namespace eetest {
namespace app_lovin {
namespace config {
// clang-format off
constexpr auto app_id = R"(gG8pkErh1_fQo-4cNDXGnxGyb9H4qz6VDEJyS8eU8IvxH-XeB4wy0BubKAg97neL0yIT4xyDEs8WqfA0l4zlGr)";
// clang-format on
} // namespace config

class Tester : public ITester {
public:
    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
    std::shared_ptr<ee::IAppLovin> plugin_;
    std::vector<std::shared_ptr<ITester>> testers_;
};
} // namespace app_lovin

using AppLovinTester = app_lovin::Tester;
} // namespace eetest

#endif /* EE_X_TEST_APP_LOVIN_TESTER_HPP */
