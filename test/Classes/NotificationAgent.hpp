//
//  NotificationAgent.hpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#ifndef EE_X_TEST_NOTIFICATION_AGENT_HPP_
#define EE_X_TEST_NOTIFICATION_AGENT_HPP_

#include <memory> // std::unique_ptr.

#include <ee/NotificationFwd.hpp>

class NotificationAgent {
public:
    static NotificationAgent* getInstance();

    void initialize();

    /// Should be called when entering background.
    /// E.g. AppDelegate::applicationDidEnterBackground.
    void scheduleAll();

    /// Should be called when entering foreground.
    /// E.g. AppDelegate::applicationWillEnterForeground.
    void unscheduleAll();

private:
    std::unique_ptr<ee::Notification> protocol_;
};

#endif /* EE_X_TEST_NOTIFICATION_AGENT_HPP_ */
