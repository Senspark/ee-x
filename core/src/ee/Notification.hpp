//
//  NotificationProtocol.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#ifndef EE_CORE_NOTIFICATION_HPP_
#define EE_CORE_NOTIFICATION_HPP_

#include <string>

namespace ee {
namespace core {
class Notification final {
private:
    static const std::string PluginName;

public:
    Notification();
    ~Notification();

    /// Schedules a notification.
    /// @param title The title of the notification.
    /// @param body The body of the notification.
    /// @param delay In seconds.
    /// @param tag The tag of the notification to differentiate with other
    /// notifications.
    void schedule(const std::string& title, const std::string& body, int delay,
                  const std::string& tag);

    /// Schedules a repeated notification.
    /// @param title The title of the notification.
    /// @param body The body of the notification.
    /// @param delay In seconds.
    /// @param interval In seconds.
    /// @param tag The tag of the notification to differentiate with other
    /// notifications.
    void schedule(const std::string& title, const std::string& body, int delay,
                  int interval, const std::string& tag);

    /// Unschedule all notifications.
    void unscheduleAll();

    /// Unschedules notifications whose the specified tag.
    void unschedule(const std::string& tag);
};
} // namespace core
} // namespace ee

#endif /* EE_CORE_NOTIFICATION_HPP_ */
