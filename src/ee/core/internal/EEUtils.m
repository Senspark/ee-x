//
//  EEUtils.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#import <Foundation/NSException.h>

#import "ee/core/internal/EEUtils.h"

#if TARGET_OS_IOS
#import <UIKit/UIKit.h>
#endif // TARGET_OS_IOS

@implementation EEUtils

#if TARGET_OS_IOS
+ (UIWindow* _Nullable)getKeyWindow {
    return [[UIApplication sharedApplication] keyWindow];
}

+ (UIViewController* _Nullable)getCurrentRootViewController {
    // [[[[UIApplication sharedApplication] delegate] window]
    // rootViewController];
    return [[self getKeyWindow] rootViewController];
}

+ (BOOL)isLandscape {
    UIInterfaceOrientation orientation =
        [[UIApplication sharedApplication] statusBarOrientation];

    return UIInterfaceOrientationIsLandscape(orientation);
}
#endif // TARGET_OS_IOS

+ (NSString* _Nonnull)toString:(BOOL)value {
    return value ? @"true" : @"false";
}

+ (BOOL)toBool:(NSString* _Nonnull)value {
    NSAssert([value isEqualToString:@"true"] ||
                 [value isEqualToString:@"false"],
             @"");
    return [value isEqualToString:@"true"];
}

@end
