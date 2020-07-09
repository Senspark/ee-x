//
//  BannerAdTester.hpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#ifndef EE_X_TEST_BANNER_AD_TESTER_HPP
#define EE_X_TEST_BANNER_AD_TESTER_HPP

#include <memory>

#include <ee/AdsFwd.hpp>

#include "ITester.hpp"

namespace eetest {
class BannerAdTester : public ITester {
public:
    explicit BannerAdTester(const std::shared_ptr<ee::IAdView>& ad);

    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
    std::shared_ptr<ee::IAdView> ad_;
};
} // namespace eetest

#endif /* EE_X_TEST_BANNER_AD_TESTER_HPP */
