//
//  EEUtils.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>

#if TARGET_OS_IOS
@class UIViewController;
@class UIWindow;
#endif // TARGET_OS_IOS

@interface EEUtils : NSObject

#if TARGET_OS_IOS
+ (UIWindow* _Nullable)getKeyWindow;
+ (UIViewController* _Nullable)getCurrentRootViewController;
+ (BOOL)isLandscape;
#endif // TARGET_OS_IOS

+ (NSString* _Nonnull)toString:(BOOL)value;
+ (BOOL)toBool:(NSString* _Nonnull)value;

@end
