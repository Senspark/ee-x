//
//  EEAppsFlyer.cpp
//  ee_x_mobile_apps_flyer
//
//  Created by eps on 6/26/18.
//

#import "ee/apps_flyer/EEAppsFlyer.h"

#import <AppsFlyerLib/AppsFlyerTracker.h>

#import <ee_x-Swift.h>

#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEMessageBridge.h>

#define kPrefix @"AppsFlyer"

// clang-format off
static NSString* const kInitialize      = kPrefix "Initialize";
static NSString* const kStartTracking   = kPrefix "StartTracking";
static NSString* const kGetDeviceId     = kPrefix "GetDeviceId";
static NSString* const kSetDebugEnabled = kPrefix "SetDebugEnabled";
static NSString* const kSetStopTracking = kPrefix "SetStopTracking";
static NSString* const kTrackEvent      = kPrefix "TrackEvent";
// clang-format on

#undef kPrefix

@interface EEAppsFlyer () <AppsFlyerTrackerDelegate>
@end

@implementation EEAppsFlyer {
    id<EEIMessageBridge> bridge_;
    AppsFlyerTracker* tracker_;
}

- (id)init {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    bridge_ = [EEMessageBridge getInstance];
    tracker_ = [AppsFlyerTracker sharedTracker];
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(applicationDidBecomeActive:)
               name:UIApplicationDidBecomeActiveNotification
             object:nil];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)destroy {
    [self deregisterHandlers];
    [[NSNotificationCenter defaultCenter]
        removeObserver:self
                  name:UIApplicationDidBecomeActiveNotification
                object:nil];
    bridge_ = nil;
    tracker_ = nil;
}

- (void)registerHandlers {
    [bridge_ registerHandler:kInitialize
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* devKey = dict[@"devKey"];
                        NSString* appId = dict[@"appId"];
                        [self initialize:devKey appId:appId];
                        return @"";
                    }];

    [bridge_ registerHandler:kStartTracking
                    callback:^(NSString* message) {
                        [self startTracking];
                        return @"";
                    }];

    [bridge_ registerHandler:kGetDeviceId
                    callback:^(NSString* message) {
                        return [self getDeviceId];
                    }];

    [bridge_ registerHandler:kSetDebugEnabled
                    callback:^(NSString* message) {
                        [self setDebugEnabled:[EEUtils toBool:message]];
                        return @"";
                    }];

    [bridge_ registerHandler:kSetStopTracking
                    callback:^(NSString* message) {
                        [self setStopTracking:[EEUtils toBool:message]];
                        return @"";
                    }];

    [bridge_ registerHandler:kTrackEvent
                    callback:^(NSString* message) {
                        NSDictionary* dict =
                            [EEJsonUtils convertStringToDictionary:message];
                        NSString* name = dict[@"name"];
                        NSDictionary* values = dict[@"values"];
                        [self trackEvent:name values:values];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:kInitialize];
    [bridge_ deregisterHandler:kStartTracking];
    [bridge_ deregisterHandler:kGetDeviceId];
    [bridge_ deregisterHandler:kSetDebugEnabled];
    [bridge_ deregisterHandler:kSetStopTracking];
    [bridge_ deregisterHandler:kTrackEvent];
}

- (void)initialize:(NSString* _Nonnull)devKey appId:(NSString* _Nonnull)appId {
    [tracker_ setAppsFlyerDevKey:devKey];
    [tracker_ setAppleAppID:appId];
    [tracker_ setDelegate:self];
    [tracker_ setShouldCollectDeviceName:YES];
    [tracker_ setDeviceTrackingDisabled:NO];
    [tracker_ trackAppLaunch];
}

- (void)startTracking {
    [tracker_ trackAppLaunch];
}

- (NSString* _Nonnull)getDeviceId {
    return [tracker_ getAppsFlyerUID];
}

- (void)setDebugEnabled:(BOOL)enabled {
    [tracker_ setIsDebug:enabled];
}

- (void)setStopTracking:(BOOL)enabled {
    [tracker_ setIsStopTracking:enabled];
}

- (void)trackEvent:(NSString* _Nonnull)name
            values:(NSDictionary* _Nonnull)dict {
    [tracker_ trackEvent:name withValues:dict];
}

- (void)applicationDidBecomeActive:(UIApplication*)application {
    // Moved to separated method.
    // [tracker_ trackAppLaunch];
}

- (void)onConversionDataSuccess:(NSDictionary*)conversionInfo {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, conversionInfo);
}

- (void)onConversionDataFail:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, error);
}

- (void)onAppOpenAttribution:(NSDictionary*)attributionData {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, attributionData);
}

- (void)onAppOpenAttributionFailure:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, error);
}

- (BOOL)application:(UIApplication* _Nonnull)application
              openURL:(NSURL* _Nonnull)url
    sourceApplication:(NSString* _Nonnull)sourceApplication
           annotation:(id _Nonnull)annotation {
    [tracker_ handleOpenURL:url
          sourceApplication:sourceApplication
             withAnnotation:annotation];
    return YES;
}

- (BOOL)application:(UIApplication* _Nonnull)application
            openURL:(NSURL* _Nonnull)url
            options:(NSDictionary* _Nonnull)options {
    [tracker_ handleOpenUrl:url options:options];
    return YES;
}

- (BOOL)application:(UIApplication* _Nonnull)application
    continueUserActivity:(NSUserActivity* _Nonnull)userActivity
      restorationHandler:(EERestorationHandler _Nonnull)restorationHandler {
    [tracker_ continueUserActivity:userActivity
                restorationHandler:restorationHandler];
    return YES;
}

@end
