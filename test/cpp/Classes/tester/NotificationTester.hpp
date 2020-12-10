//
//  NotificationAgent.hpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#ifndef EE_X_TEST_NOTIFICATION_TESTER_HPP
#define EE_X_TEST_NOTIFICATION_TESTER_HPP

#include <memory>

#include <ee/NotificationFwd.hpp>

#include "ITester.hpp"

namespace eetest {
namespace notification {
class Tester : public ITester {
public:
    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
    std::shared_ptr<ee::Notification> plugin_;
};
} // namespace notification

using NotificationTester = notification::Tester;
} // namespace eetest

#endif /* EE_X_TEST_NOTIFICATION_TESTER_HPP */
