//
//  EEAppsFlyer.cpp
//  ee_x_mobile_apps_flyer
//
//  Created by eps on 6/26/18.
//

#import "ee/appsflyer/EEAppsFlyer.h"

#import <AppsFlyerLib/AppsFlyerTracker.h>

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"

// clang-format off
static NSString* const kInitialize      = @"AppsFlyerInitialize";
static NSString* const kGetDeviceId     = @"AppsFlyerGetDeviceId";
static NSString* const kSetDebugEnabled = @"AppsFlyerSetDebugEnabled";
static NSString* const kTrackEvent      = @"AppsFlyerTrackEvent";
// clang-format on

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
    return self;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter]
        removeObserver:self
                  name:UIApplicationDidBecomeActiveNotification
                object:nil];
    [super dealloc];
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

    [bridge_ registerHandler:kGetDeviceId
                    callback:^(NSString* message) {
                        return [self getDeviceId];
                    }];

    [bridge_ registerHandler:kSetDebugEnabled
                    callback:^(NSString* message) {
                        [self setDebugEnabled:[EEUtils toBool:message]];
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
    [bridge_ deregisterHandler:kGetDeviceId];
    [bridge_ deregisterHandler:kSetDebugEnabled];
    [bridge_ deregisterHandler:kTrackEvent];
}

- (void)initialize:(NSString* _Nonnull)devKey appId:(NSString* _Nonnull)appId {
    [tracker_ setAppsFlyerDevKey:devKey];
    [tracker_ setAppleAppID:appId];
    [tracker_ setShouldCollectDeviceName:YES];
}

- (NSString* _Nonnull)getDeviceId {
    return [tracker_ getAppsFlyerUID];
}

- (void)setDebugEnabled:(BOOL)enabled {
    [tracker_ setIsDebug:enabled];
}

- (void)trackEvent:(NSString* _Nonnull)name
            values:(NSDictionary* _Nonnull)dict {
    [tracker_ trackEvent:name withValues:dict];
}

- (void)applicationDidBecomeActive:(UIApplication*)application {
    [tracker_ trackAppLaunch];
}

- (void)onConversionDataReceived:(NSDictionary*)installData {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, installData);
}

- (void)onConversionDataRequestFailure:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, error);
}

- (void)onAppOpenAttribution:(NSDictionary*)attributionData {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, attributionData);
}

- (void)onAppOpenAttributionFailure:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, error);
}

@end
