//
//  Notification.hpp
//  ee_x
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_NOTIFICATION_HEADER_HPP_
#define EE_X_NOTIFICATION_HEADER_HPP_

#include "ee/notification/Notification.hpp"
#include "ee/notification/NotificationBuilder.hpp"

#ifdef __OBJC__
#import "ee/notification/EENotification.h"
#endif // __OBJC__

namespace ee {
using notification::Notification;
using notification::NotificationBuilder;
} // namespace ee

#endif /* EE_X_NOTIFICATION_HEADER_HPP_ */
