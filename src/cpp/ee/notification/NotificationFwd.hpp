//
//  NotificationFwd.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_NOTIFICATION_FWD_HPP
#define EE_X_NOTIFICATION_FWD_HPP

#ifdef __cplusplus

#include <ee/core/CoreFwd.hpp>

namespace ee {
namespace notification {
class IBridge;
class Bridge;
class NotificationBuilder;
} // namespace notification

using INotification = notification::IBridge;
using notification::NotificationBuilder;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_NOTIFICATION_FWD_HPP */
