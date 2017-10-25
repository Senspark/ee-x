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
static NSString* const k__getSHA1CertificateFingerprint = @"Utils_getSHA1CertificateFingerprint";
static NSString* const k__getVersionName                = @"Utils_getVersionName";
static NSString* const k__getVersionCode                = @"Utils_getVersionCode";
static NSString* const k__testConnection                = @"Utils_testConnection";
// clang-format on

+ (void)initializeHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
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

    [bridge registerHandler:k__testConnection
                   callback:^(NSString* message) {
                       NSString* hostName = message;
                       return [self toString:[self testConnection:hostName]];
                   }];
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

+ (BOOL)testConnection:(NSString* _Nonnull)hostName {
    return [[EEReachability reachabilityWithHostname:hostName] isReachable];
}

@end
