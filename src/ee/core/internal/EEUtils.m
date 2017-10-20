//
//  EEUtils.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#import "ee/core/internal/EEUtils.h"

#if TARGET_OS_IOS
#import <UIKit/UIKit.h>
#endif // TARGET_OS_IOS

@implementation EEUtils

// clang-format off
static NSString* const k__true  = @"true";
static NSString* const k__false = @"false";
// clang-format on

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
    return value ? k__true : k__false;
}

+ (BOOL)toBool:(NSString* _Nonnull)value {
    if (![value isEqualToString:k__true] && ![value isEqualToString:k__false]) {
        NSString* message =
            [NSString stringWithFormat:@"Unexpected value: %@", value];
        NSAssert(NO, message);
    }
    return [value isEqualToString:k__true];
}

@end
