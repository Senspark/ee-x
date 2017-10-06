//
//  EEMetrics.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <TargetConditionals.h>

#if TARGET_OS_IOS
#import <UIKit/UIScreen.h>
#else // TARGET_OS_IOS
#import <AppKit/NSScreen.h>
#endif // TARGET_OS_IOS

#import "ee/core/EEMetrics.h"
#import "ee/core/internal/EEMessageBridge.h"

@implementation EEMetrics

NSString* const k__getDensity = @"Metrics_getDensity";

+ (void)initializeMetrics {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge registerHandler:k__getDensity
                   callback:^(NSString* message) {
                       CGFloat density = [self getDensity];
                       return [NSString stringWithFormat:@"%f", density];
                   }];
}

+ (CGFloat)getDensity {
    CGFloat density = 1.0f;
#if TARGET_OS_IOS
    if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
        density = [[UIScreen mainScreen] scale];
    }
#else  // TARGET_OS_IOS
    if ([[NSScreen mainScreen]
            respondsToSelector:@selector(backingScaleFactor)]) {
        for (NSScreen* screen in [NSScreen screens]) {
            CGFloat scale = [screen backingScaleFactor];
            if (scale > density) {
                density = scale;
            }
        }
    }
#endif // TARGET_OS_IOS
    return density;
}

@end
