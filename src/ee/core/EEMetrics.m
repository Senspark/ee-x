//
//  EEMetrics.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <UIKit/UIScreen.h>

#import "ee/core/EEMetrics.h"
#import "ee/core/internal/EEMessageBridge.h"

@implementation EEMetrics

NSString* const k__getDensity = @"Metrics_getDensity";

+ (void)initialize {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge registerHandler:k__getDensity
                   callback:^(NSString* message) {
                       CGFloat density = [self getDensity];
                       return [NSString stringWithFormat:@"%f", density];
                   }];
}

+ (CGFloat)getDensity {
    CGFloat density = 1.0f;
    if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
        density = [[UIScreen mainScreen] scale];
    }
    return density;
}

@end
