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
@class UIWindow;
#endif // TARGET_OS_IOS

@class RACSignal;

@interface EEUtils : NSObject

+ (void)registerHandlers;

#if TARGET_OS_IOS
+ (UIWindow* _Nullable)getKeyWindow;
+ (UIViewController* _Nullable)getCurrentRootViewController;

/// Checks whether the device orientation is landscape.
+ (BOOL)isLandscape;
#endif // TARGET_OS_IOS

/// Converts a bool value to string.
+ (NSString* _Nonnull)toString:(BOOL)value;

/// Converts a string value to bool.
+ (BOOL)toBool:(NSString* _Nonnull)value;

+ (BOOL)isMainThread;
+ (BOOL)runOnMainThread:(void (^_Nonnull)(void))callback;
+ (void)runOnMainThreadDelayed:(float)seconds
                      callback:(void (^_Nonnull)(void))callback;

+ (BOOL)isApplicationInstalled:(NSString* _Nonnull)applicationId;
+ (BOOL)openApplication:(NSString* _Nonnull)applicationId;

+ (NSString* _Nonnull)getApplicationId;
+ (NSString* _Nonnull)getApplicationName;
+ (NSString* _Nonnull)getVersionName;
+ (NSString* _Nonnull)getVersionCode;

+ (BOOL)isTablet;
+ (CGFloat)getDensity;
+ (CGFloat)convertDpToPixels:(CGFloat)dp;
+ (CGFloat)convertPixelsToDp:(CGFloat)pixels;
+ (NSString* _Nonnull)getDeviceId;

+ (BOOL)sendMail:(NSString* _Nonnull)recipient
         subject:(NSString* _Nonnull)subject
            body:(NSString* _Nonnull)body;

+ (RACSignal* _Nonnull)testConnection:(NSString* _Nonnull)hostName
                              timeOut:(float)timeOut;

@end
