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
             tag:(NSString*)tag;

- (void)schedule:(NSString*)title
            body:(NSString*)body
           delay:(NSTimeInterval)delay
        interval:(NSCalendarUnit)interval
             tag:(NSString*)tag;

- (void)unscheduleAll;

- (void)unschedule:(NSString*)tag;

@end
