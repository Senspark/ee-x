//
//  EENotification.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

@protocol EEIPlugin;

@interface EENotification : NSObject <EEIPlugin>

- (void)schedule:(NSString*)title
            body:(NSString*)body
           delay:(NSTimeInterval)delay
        interval:(NSCalendarUnit)interval
             tag:(NSNumber*)tag;

- (void)unschedule:(NSNumber*)tag;

- (void)unscheduleAll;

- (void)clearAll;

@end
