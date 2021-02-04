//
//  NotficationBridge.swift
//  ee-x-bb8dc50f
//
//  Created by eps on 2/4/21.
//

import Foundation

private let kTag = "\(NotificationBridge.self)"
private let kPrefix = "NotificationBridge"
private let kSchedule = "\(kPrefix)Schedule"
private let kUnschedule = "\(kPrefix)Unschedule"
private let kClearAll = "\(kPrefix)ClearAll"

@objc(EENotification)
class NotificationBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        super.init()
        registerHandlers()
        registerForLocalNotifications()
    }

    public func destroy() {
        deregisterHandlers()
    }

    func registerHandlers() {
        _bridge.registerHandler(kSchedule) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let title = dict["title"] as? String,
                let body = dict["body"] as? String,
                let delay = dict["delay"] as? Int,
                let interval = dict["interval"] as? Int,
                let tag = dict["tag"] as? Int
            else {
                assert(false, "Invalid argument")
                return ""
            }
            self.schedule(title, body, TimeInterval(delay), self.parseInterval(interval), tag)
            return ""
        }
        _bridge.registerHandler(kUnschedule) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard let tag = dict["tag"] as? Int else {
                assert(false, "Invalid argument")
                return ""
            }
            self.unschedule(tag)
            return ""
        }
        _bridge.registerHandler(kClearAll) { _ in
            self.clearAll()
            return ""
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kSchedule)
        _bridge.deregisterHandler(kUnschedule)
        _bridge.deregisterHandler(kClearAll)
    }

    func registerForLocalNotifications() {
        let types = UIUserNotificationType.sound
            .union(UIUserNotificationType.alert)
            .union(UIUserNotificationType.badge)
        let settings = UIUserNotificationSettings(types: types, categories: nil)
        UIApplication.shared.registerUserNotificationSettings(settings)
    }

    func parseInterval(_ interval: Int) -> NSCalendar.Unit {
        if interval == 0 {
            return NSCalendar.Unit(rawValue: 0)
        }
        if interval <= 1 {
            return NSCalendar.Unit.second
        }
        if interval <= 60 {
            return NSCalendar.Unit.minute
        }
        if interval <= 3600 {
            return NSCalendar.Unit.hour
        }
        if interval <= 86400 {
            return NSCalendar.Unit.day
        }
        return NSCalendar.Unit.era
    }

    func createNotification(
        _ fireDate: Date,
        _ interval: NSCalendar.Unit,
        _ title: String,
        _ body: String) -> UILocalNotification {
        let notification = UILocalNotification()
        notification.alertTitle = title
        notification.fireDate = fireDate
        notification.repeatInterval = interval
        // http://stackoverflow.com/questions/5985468/iphone-differences-among-time-zone-convenience-methods?noredirect=1&lq=1
        notification.timeZone = NSTimeZone.local
        notification.soundName = UILocalNotificationDefaultSoundName
        notification.alertBody = body
        return notification
    }

    func schedule(
        _ title: String,
        _ body: String,
        _ delay: TimeInterval,
        _ interval: NSCalendar.Unit,
        _ tag: Int) {
        let notification = createNotification(Date(timeIntervalSinceNow: delay), interval, title, body)
        notification.userInfo = ["notification_tag": tag]
        UIApplication.shared.scheduleLocalNotification(notification)
    }

    func unschedule(_ tag: Int) {
        let application = UIApplication.shared
        let notifications = application.scheduledLocalNotifications
        notifications?.forEach { notification in
            guard let notificationTag = notification.userInfo?["notification_tag"] as? Int else {
                return
            }
            if tag == notificationTag {
                application.cancelLocalNotification(notification)
            }
        }
    }

    func clearAll() {
        // http://stackoverflow.com/questions/8682051/ios-application-how-to-clear-notifications
        UIApplication.shared.applicationIconBadgeNumber = 1
        UIApplication.shared.applicationIconBadgeNumber = 0
        UIApplication.shared.cancelAllLocalNotifications()
    }
}
