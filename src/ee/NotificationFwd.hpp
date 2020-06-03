//
//  NotificationFwd.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_NOTIFICATION_FWD_HPP_
#define EE_X_NOTIFICATION_FWD_HPP_

#ifdef __cplusplus

#include <ee/CoreFwd.hpp>

namespace ee {
namespace notification {
class Bridge;
class NotificationBuilder;
} // namespace notification

using Notification = notification::Bridge;
using notification::NotificationBuilder;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_NOTIFICATION_FWD_HPP_ */
