//
//  EEUtils.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#import "ee/core/internal/EEUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEReachability.h"

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

// clang-format off
static NSString* const k__isMainThread                  = @"Utils_isMainThread";
static NSString* const k__runOnUiThread                 = @"Utils_runOnUiThread";
static NSString* const k__runOnUiThreadCallback         = @"Utils_runOnUiThreadCallback";
static NSString* const k__getSHA1CertificateFingerprint = @"Utils_getSHA1CertificateFingerprint";
static NSString* const k__getVersionName                = @"Utils_getVersionName";
static NSString* const k__getVersionCode                = @"Utils_getVersionCode";
static NSString* const k__isApplicationInstalled        = @"Utils_isApplicationInstalled";
static NSString* const k__openApplication               = @"Utils_openApplication";
static NSString* const k__isTablet                      = @"Utils_isTablet";
static NSString* const k__testConnection                = @"Utils_testConnection";
// clang-format on

+ (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__isMainThread
                   callback:^(NSString* message) {
                       return [self toString:[self isMainThread]];
                   }];
    [bridge registerHandler:k__runOnUiThread
                   callback:^(NSString* message) {
                       return [self toString:[self runOnMainThread]];
                   }];

    [bridge registerHandler:k__getSHA1CertificateFingerprint
                   callback:^(NSString* message) {
                       // Not supported.
                       return @"";
                   }];

    [bridge registerHandler:k__getVersionName
                   callback:^(NSString* message) {
                       return [self getVersionName];
                   }];

    [bridge registerHandler:k__getVersionCode
                   callback:^(NSString* message) {
                       return [self getVersionCode];
                   }];

    [bridge
        registerHandler:k__isApplicationInstalled
               callback:^(NSString* message) {
                   NSString* applicationId = message;
                   return [self
                       toString:[self isApplicationInstalled:applicationId]];
               }];

    [bridge registerHandler:k__openApplication
                   callback:^(NSString* message) {
                       NSString* applicationId = message;
                       return
                           [self toString:[self openApplication:applicationId]];
                   }];

    [bridge registerHandler:k__isTablet
                   callback:^(NSString* message) {
                       return [self toString:[self isTablet]];
                   }];

    [bridge registerHandler:k__testConnection
                   callback:^(NSString* message) {
                       return [self
                           toString:[self testConnection:@"www.google.com"]];
                   }];
}

+ (BOOL)isMainThread {
    return [NSThread isMainThread];
}

+ (void)signalMainThread {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__runOnUiThreadCallback];
}

+ (BOOL)runOnMainThread {
    if ([self isMainThread]) {
        [self signalMainThread];
        return YES;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        [self signalMainThread];
    });
    return NO;
}

+ (NSString* _Nonnull)getVersionName {
    NSString* versionName = [[NSBundle mainBundle]
        objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
    return versionName;
}

+ (NSString* _Nonnull)getVersionCode {
    NSString* versionString = [[NSBundle mainBundle]
        objectForInfoDictionaryKey:(NSString*)kCFBundleVersionKey];
    return versionString;
}

+ (BOOL)isApplicationInstalled:(NSString* _Nonnull)applicationId {
#if TARGET_OS_IOS
    NSURL* url =
        [NSURL URLWithString:[applicationId stringByAppendingString:@"://"]];
    return [[UIApplication sharedApplication] canOpenURL:url];
#else  // TARGET_OS_IOS
    // macOS
    return NO;
#endif // TARGET_OS_IOS
}

+ (BOOL)openApplication:(NSString* _Nonnull)applicationId {
#if TARGET_OS_IOS
    NSURL* url =
        [NSURL URLWithString:[applicationId stringByAppendingString:@"://"]];
    return [[UIApplication sharedApplication] openURL:url];
#else  // TARGET_OS_IOS
    return NO;
#endif // TARGET_OS_IOS
}

+ (BOOL)isTablet {
#if TARGET_OS_IOS
    return [[[UIDevice currentDevice] model] hasPrefix:@"iPad"];
#else  // TARGET_OS_IOS
    return NO;
#endif // TARGET_OS_IOS
}

+ (BOOL)testConnection:(NSString* _Nonnull)hostName {
    return [[EEReachability reachabilityWithHostname:hostName] isReachable];
}

@end
