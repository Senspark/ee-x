//
//  InterstitialAdTester.hpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#ifndef EE_X_TEST_INTERSTITIAL_AD_TESTER_HPP
#define EE_X_TEST_INTERSTITIAL_AD_TESTER_HPP

#include <memory>

#include <ee/CppFwd.hpp>

#include "ITester.hpp"

namespace eetest {
class InterstitialAdTester : public ITester {
public:
    explicit InterstitialAdTester(
        const std::shared_ptr<ee::IInterstitialAd>& ad);

    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
    std::shared_ptr<ee::IInterstitialAd> ad_;
};
} // namespace eetest

#endif /* EE_X_TEST_INTERSTITIAL_AD_TESTER_HPP */
