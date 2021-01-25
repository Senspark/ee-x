//
//  NotificationProtocol.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#ifndef EE_X_NOTIFICATION_BRIDGE_HPP
#define EE_X_NOTIFICATION_BRIDGE_HPP

#include "ee/notification/INotificationBridge.hpp"

namespace ee {
namespace notification {
class Bridge final : public IBridge {
public:
    explicit Bridge(IMessageBridge& bridge);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual void schedule(const NotificationBuilder& builder) override;
    virtual void schedule(const std::string& message, int tag, int delay,
                          int interval) override;

    virtual void unschedule(int tag) override;
    virtual void clearAll() override;

private:
    IMessageBridge& bridge_;
};
} // namespace notification
} // namespace ee

#endif /* EE_X_NOTIFICATION_BRIDGE_HPP */
