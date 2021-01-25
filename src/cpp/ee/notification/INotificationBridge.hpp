#ifndef EE_X_I_NOTIFICATION_BRIDGE_HPP
#define EE_X_I_NOTIFICATION_BRIDGE_HPP

#ifdef __cplusplus

#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/notification/NotificationFwd.hpp"

namespace ee {
namespace notification {
class IBridge : public IPlugin {
public:
    /// Schedules a notification.
    virtual void schedule(const NotificationBuilder& builder) = 0;
    virtual void schedule(const std::string& message, int tag, int delay,
                          int interval) = 0;

    /// Unschedules notifications whose the specified tag.
    virtual void unschedule(int tag) = 0;

    /// Clear all notifications.
    virtual void clearAll() = 0;
};
} // namespace notification
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_NOTIFICATION_BRIDGE_HPP