//
//  EENotification.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#import <Foundation/Foundation.h>

@interface EENotification : NSObject

- (void)schedule:(NSString*)title
            body:(NSString*)body
           delay:(NSTimeInterval)delay
             tag:(NSNumber*)tag;

- (void)schedule:(NSString*)title
            body:(NSString*)body
           delay:(NSTimeInterval)delay
        interval:(NSCalendarUnit)interval
             tag:(NSNumber*)tag;

- (void)unschedule:(NSNumber*)tag;

- (void)unscheduleAll;

@end
