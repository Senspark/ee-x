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

#if TARGET_OS_IOS
- (UIViewController* _Nullable)getCurrentRootViewController {
    // [[[[UIApplication sharedApplication] delegate] window]
    // rootViewController];
    return [[[UIApplication sharedApplication] keyWindow] rootViewController];
}
#endif // TARGET_OS_IOS

@end
