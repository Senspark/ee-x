//
//  GoogleAnalyticsTest.hpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#ifndef EE_X_TEST_GOOGLE_ANALYTICS_TESTER_HPP
#define EE_X_TEST_GOOGLE_ANALYTICS_TESTER_HPP

#include <ee/GoogleAnalyticsFwd.hpp>

#include "ITester.hpp"

namespace eetest {
namespace google {
class Tester : public ITester {
public:
    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
    std::shared_ptr<ee::GoogleAnalytics> plugin_;
};
} // namespace google

using GoogleAnalyticsTester = google::Tester;
} // namespace eetest

#endif /* EE_X_TEST_GOOGLE_ANALYTICS_TESTER_HPP */
