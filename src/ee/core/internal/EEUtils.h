//
//  EEUtils.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#import <Foundation/Foundation.h>

#if TARGET_OS_IOS
@class UIViewController;
#endif // TARGET_OS_IOS

@interface EEUtils : NSObject

#if TARGET_OS_IOS
+ (UIViewController* _Nullable)getCurrentRootViewController;
#endif // TARGET_OS_IOS

@end
