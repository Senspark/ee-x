//
//  EENotification.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#import "ee/internal/EENotification.h"
#import "ee/internal/EEDictionaryUtils.h"
#import "ee/internal/EEJsonUtils.h"
#import "ee/internal/EEMessageBridge.h"

#if TARGET_OS_IOS
#import <UIKit/UIKit.h>
#endif // TARGET_OS_IOS

@implementation EENotification

NSString* const k__notification_schedule = @"__notification_schedule";
NSString* const k__notification_unschedule_all =
    @"__notification_unschedule_all";
NSString* const k__notification_unschedule = @"__notification_unschedule";

- (id)init {
    self = [super init];
    if (self == nil) {
        return nil;
    }

    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];
        NSString* title = [dict objectForKey:@"title"];
        NSString* body = [dict objectForKey:@"body"];
        NSNumber* delay = [dict objectForKey:@"delay"];
        NSNumber* interval = [dict objectForKey:@"interval"];
        NSString* tag = [dict objectForKey:@"tag"];
        return [EEDictionaryUtils emptyResult];
    } tag:k__notification_schedule];

    [bridge registerHandler:^(NSString* msg) {
        [self unscheduleAll];
        return [EEDictionaryUtils emptyResult];
    } tag:k__notification_unschedule_all];

    [bridge registerHandler:^(NSString* msg) {
        [self unschedule:msg];
        return [EEDictionaryUtils emptyResult];
    } tag:k__notification_unschedule];

    return self;
}

- (void)dealloc {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__notification_schedule];
    [bridge deregisterHandler:k__notification_unschedule_all];
    [bridge deregisterHandler:k__notification_unschedule];

    [super dealloc];
}

#if TARGET_OS_IOS
- (UILocalNotification*)create:(NSDate*)fireDate
                      interval:(NSCalendarUnit)interval
                         title:(NSString*)title
                          body:(NSString*)body {
    UILocalNotification* notification =
        [[[UILocalNotification alloc] init] autorelease];
    if (notification == nil) {
        return nil;
    }
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
        [notification setAlertTitle:title];
    }

    [notification setFireDate:fireDate];
    [notification setRepeatInterval:interval];
    [notification setTimeZone:[NSTimeZone defaultTimeZone]];
    [notification setSoundName:UILocalNotificationDefaultSoundName];
    [notification setAlertBody:body];
    return notification;
}
#endif // TARGET_OS_IOS

- (void)schedule:(NSString*)title
            body:(NSString*)body
           delay:(NSTimeInterval)delay
             tag:(NSString*)tag {
    [self schedule:title
              body:body
             delay:delay
          interval:(NSCalendarUnit)0
               tag:tag];
}
- (void)schedule:(NSString*)title
            body:(NSString*)body
           delay:(NSTimeInterval)delay
        interval:(NSCalendarUnit)interval
             tag:(NSString*)tag {
#if TARGET_OS_IOS
    UILocalNotification* notification =
        [self create:[NSDate dateWithTimeIntervalSinceNow:delay]
            interval:interval
               title:title
                body:body];
    if (notification == nil) {
        return;
    }

    NSDictionary* dict =
        [NSDictionary dictionaryWithObject:tag forKey:@"notification_tag"];
    [notification setUserInfo:dict];

    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
#endif // TARGET_OS_IOS
}

- (void)unscheduleAll {
#if TARGET_OS_IOS
    UIApplication* application = [UIApplication sharedApplication];
    NSArray* notifications = [application scheduledLocalNotifications];

    if ([notifications count] > 0) {
        [application cancelAllLocalNotifications];
    }
#endif // TARGET_OS_IOS
}

- (void)unschedule:(NSString*)tag {
#if TARGET_OS_IOS
    UIApplication* application = [UIApplication sharedApplication];
    NSArray* notifications = [application scheduledLocalNotifications];

    for (UILocalNotification* notification in notifications) {
        if ([[[notification userInfo] objectForKey:@"notification_tag"]
                isEqualToString:tag]) {
            [application cancelLocalNotification:notification];
        }
    }
#endif // TARGET_OS_IOS
}

@end
