//
//  EEUtils.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#import "ee/core/internal/EEUtils.h"

#import <ReactiveObjC/ReactiveObjC.h>
#import <TargetConditionals.h>

#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/private/EEReachability.h"

#if TARGET_OS_IOS
#import <UIKit/UIKit.h>
#endif // TARGET_OS_IOS

#if TARGET_OS_IOS
#import <UIKit/UIScreen.h>
#else // TARGET_OS_IOS
#import <AppKit/NSScreen.h>
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

+ (BOOL)isMainThread {
    return [NSThread isMainThread];
}

+ (void)signalMainThread {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__runOnUiThreadCallback];
}

+ (BOOL)runOnMainThread:(void (^_Nonnull)(void))callback {
    if ([self isMainThread]) {
        callback();
        return YES;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        callback();
    });
    return NO;
}

+ (void)runOnMainThreadDelayed:(float)seconds
                      callback:(void (^_Nonnull)(void))callback {
    dispatch_time_t popTime =
        dispatch_time(DISPATCH_TIME_NOW, (int64_t)(seconds * NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void) {
        callback();
    });
}

// clang-format off
static NSString* const k__isMainThread                  = @"Utils_isMainThread";
static NSString* const k__runOnUiThread                 = @"Utils_runOnUiThread";
static NSString* const k__runOnUiThreadDelayed          = @"Utils_runOnUiThreadDelayed";
static NSString* const k__runOnUiThreadCallback         = @"Utils_runOnUiThreadCallback";

static NSString* const k__isApplicationInstalled        = @"Utils_isApplicationInstalled";
static NSString* const k__openApplication               = @"Utils_openApplication";

static NSString* const k__getApplicationId              = @"Utils_getApplicationId";
static NSString* const k__getApplicationName            = @"Utils_getApplicationName";
static NSString* const k__getVersionName                = @"Utils_getVersionName";
static NSString* const k__getVersionCode                = @"Utils_getVersionCode";

static NSString* const k__isTablet                      = @"Utils_isTablet";
static NSString* const k__getDensity                    = @"Utils_getDensity";
static NSString* const k__getDeviceId                   = @"Utils_getDeviceId";

static NSString* const k__sendMail                      = @"Utils_sendMail";
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
                       return [self toString:[self runOnMainThread:^{
                                        [self signalMainThread];
                                    }]];
                   }];

    [bridge registerHandler:k__runOnUiThreadDelayed
                   callback:^NSString* _Nonnull(NSString* _Nonnull message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* tag = dict[@"callback_id"];
                       float delay = [dict[@"delay_time"] floatValue];
                       [self runOnMainThreadDelayed:delay
                                           callback:^{
                                               [bridge callCpp:tag];
                                           }];
                       return @"";
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

    [bridge registerHandler:k__getApplicationId
                   callback:^(NSString* message) {
                       return [self getApplicationId];
                   }];

    [bridge registerHandler:k__getApplicationName
                   callback:^(NSString* message) {
                       return [self getApplicationName];
                   }];

    [bridge registerHandler:k__getVersionName
                   callback:^(NSString* message) {
                       return [self getVersionName];
                   }];

    [bridge registerHandler:k__getVersionCode
                   callback:^(NSString* message) {
                       return [self getVersionCode];
                   }];

    [bridge registerHandler:k__isTablet
                   callback:^(NSString* message) {
                       return [self toString:[self isTablet]];
                   }];

    [bridge registerHandler:k__getDensity
                   callback:^(NSString* message) {
                       CGFloat density = [self getDensity];
                       return [NSString stringWithFormat:@"%f", density];
                   }];

    [bridge registerHandler:k__getDeviceId
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* callbackTag = dict[@"callback_tag"];
                       NSAssert(callbackTag != nil, @"");

                       NSString* deviceId = [self getDeviceId];
                       [bridge callCpp:callbackTag message:deviceId];
                       return @"";
                   }];

    [bridge registerHandler:k__sendMail
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* recipient = dict[@"recipient"];
                       NSString* subject = dict[@"subject"];
                       NSString* body = dict[@"body"];
                       return [self toString:[self sendMail:recipient
                                                    subject:subject
                                                       body:body]];
                   }];
    [bridge registerHandler:k__testConnection
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];

                       NSString* callbackTag = dict[@"callback_tag"];
                       NSString* hostName = dict[@"host_name"];
                       float timeOut = [dict[@"time_out"] floatValue];
                       NSAssert(callbackTag != nil, @"");
                       NSAssert(hostName != nil, @"");

                       [[self testConnection:hostName timeOut:timeOut]
                           subscribeNext:^(id _Nullable x) {
                               BOOL result = [x boolValue];
                               [bridge callCpp:callbackTag
                                       message:[self toString:result]];
                           }
                           error:^(NSError* _Nullable error) {
                               [bridge callCpp:callbackTag
                                       message:[self toString:NO]];
                           }];
                       return @"";
                   }];
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

+ (NSString* _Nonnull)getApplicationId {
    return [[[NSBundle mainBundle] infoDictionary]
        objectForKey:(NSString*)kCFBundleIdentifierKey];
}

+ (NSString* _Nonnull)getApplicationName {
    return [[[NSBundle mainBundle] infoDictionary]
        objectForKey:(NSString*)kCFBundleNameKey];
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

+ (BOOL)isTablet {
#if TARGET_OS_IOS
    return [[[UIDevice currentDevice] model] hasPrefix:@"iPad"];
#else  // TARGET_OS_IOS
    return NO;
#endif // TARGET_OS_IOS
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

+ (CGFloat)convertDpToPixels:(CGFloat)dp {
    return dp * [self getDensity];
}

+ (CGFloat)convertPixelsToDp:(CGFloat)pixels {
    return pixels / [self getDensity];
}

+ (NSString* _Nonnull)getDeviceId {
#if TARGET_OS_IOS
    // https://stackoverflow.com/questions/24760150/how-to-get-a-hashed-device-id-for-testing-admob-on-ios
    NSString* deviceId =
        [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    return deviceId == nil ? @"" : deviceId;
#else  // TARGET_OS_IOS
    // https://stackoverflow.com/questions/933460/unique-hardware-id-in-mac-os-x
    io_registry_entry_t ioRegistryRoot =
        IORegistryEntryFromPath(kIOMasterPortDefault, "IOService:/");
    CFStringRef uuidCf = (CFStringRef)IORegistryEntryCreateCFProperty(
        ioRegistryRoot, CFSTR(kIOPlatformUUIDKey), kCFAllocatorDefault, 0);
    IOObjectRelease(ioRegistryRoot);
    const auto CFIndex bufSize = 100;
    static char buf[bufSize];
    Boolean successful =
        CFStringGetCString(uuidCf, buf, bufSize, kCFStringEncodingMacRoman);
    CFRelease(uuidCf);
    if (successful) {
        NSString* deviceId = [NSString stringWithUTF8String:buf];
        return deviceId == nil ? @"" : deviceId;
    }
    return @"";
#endif // TARGET_OS_IOS
}

+ (BOOL)sendMail:(NSString* _Nonnull)recipient
         subject:(NSString* _Nonnull)subject
            body:(NSString* _Nonnull)body {
#if TARGET_OS_IOS
    NSString* str = [NSString stringWithFormat:@"mailto:%@?subject=%@&body=%@",
                                               recipient, subject, body];
    NSURL* url =
        [NSURL URLWithString:[str stringByAddingPercentEscapesUsingEncoding:
                                      NSUTF8StringEncoding]];
    return [[UIApplication sharedApplication] openURL:url];
#else  // TARGET_OS_IOS
    return NO;
#endif // TARGET_OS_IOS
}

+ (RACSignal* _Nonnull)testConnection:(NSString* _Nonnull)hostName
                              timeOut:(float)timeOut {
    RACScheduler* scheduler =
        [RACScheduler schedulerWithPriority:RACSchedulerPriorityBackground];
    return [[RACSignal
        startLazilyWithScheduler:scheduler
                           block:^(id<RACSubscriber> _Nonnull subscriber) {
                               NetworkStatus status = [[EEReachability
                                   reachabilityWithHostname:hostName]
                                   currentReachabilityStatus];
                               BOOL isReachable = status != NotReachable;
                               [subscriber sendNext:@(isReachable)];
                               [subscriber sendCompleted];
                           }] timeout:timeOut onScheduler:scheduler];
}

@end
