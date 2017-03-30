//
//  NotificationProtocol.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#ifndef EE_X_NOTIFICATION_HPP_
#define EE_X_NOTIFICATION_HPP_

#include <string>

namespace ee {
class Notification final {
public:
    /// Schedules a notification.
    /// @param title The title of the notification.
    /// @param body The body of the notification.
    /// @param delay In seconds.
    /// @param tag The tag of the notification to differentiate with other
    /// notifications.
    void schedule(const std::string& title, const std::string& body, int delay,
                  int tag);

    /// Schedules a repeated notification.
    /// @param title The title of the notification.
    /// @param body The body of the notification.
    /// @param delay In seconds.
    /// @param interval In seconds.
    /// @param tag The tag of the notification to differentiate with other
    /// notifications.
    void schedule(const std::string& title, const std::string& body, int delay,
                  int interval, int tag);

    /// Unschedules notifications whose the specified tag.
    void unschedule(int tag);

private:
    /// Unschedule all notifications.
    /// Only supported on iOS.
    void unscheduleAll();
};
} // namespace ee

#endif /* EE_X_NOTIFICATION_HPP_ */
