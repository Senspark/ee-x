//
//  EEVungle.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <UIKit/UIKit.h>
#import <VungleSDK/VungleSDK.h>

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/vungle/EEVungle.h"

#define EE_VUNGLE_VERSION_4

@interface EEVungle () <VungleSDKDelegate>
@end

@implementation EEVungle

// clang-format off
NSString* const k__VungleAds_initVungleAds  = @"k__VungleAds_initVungleAds";
NSString* const k__VungleAds_isAdReady      = @"k__VungleAds_isAdReady";
NSString* const k__VungleAds_showAds        = @"k__VungleAds_showAds";
NSString* const k__Vungle_cpp_callback      = @"__VungleAds_callback";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    VungleSDK* sdk = [VungleSDK sharedSDK];
    [sdk setDelegate:nil];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* gameId = dict[@"GameID"];
        NSArray* placementIds = dict[@"PlacementIDs"];
        [self initialize:gameId placementIds:placementIds];
        return @"";
    }
                        tag:k__VungleAds_initVungleAds];

    [bridge registerHandler:^(NSString* msg) {
        NSString* placementId = msg;
        return [self isAdReady:placementId] ? @"true" : @"false";
    }
                        tag:k__VungleAds_isAdReady];

    [bridge registerHandler:^(NSString* msg) {
        NSString* placementId = msg;
        [self showAds:placementId];
        return @"";
    }
                        tag:k__VungleAds_showAds];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__VungleAds_initVungleAds];
    [bridge deregisterHandler:k__VungleAds_isAdReady];
    [bridge deregisterHandler:k__VungleAds_showAds];
}

- (void)initialize:(NSString*)gameId placementIds:(NSArray*)placementIds {
    VungleSDK* sdk = [VungleSDK sharedSDK];

#ifdef EE_VUNGLE_VERSION_4
    [sdk startWithAppId:gameId];
#else  // EE_VUNGLE_VERSION_4
    [sdk startWithAppId:gameId placements:placementIds error:nil];
#endif // placementIds

    [sdk setLoggingEnabled:YES];
    [sdk setDelegate:self];
}

- (BOOL)isAdReady:(NSString*)placementId {
    VungleSDK* sdk = [VungleSDK sharedSDK];
#ifdef EE_VUNGLE_VERSION_4
    return [sdk isAdPlayable];
#else  // EE_VUNGLE_VERSION_4
    return [sdk isAdCachedForPlacementID:placementId];
#endif // EE_VUNGLE_VERSION_4
}

- (BOOL)showAds:(NSString*)placementId {
    UIViewController* view = [EEUtils getCurrentRootViewController];
    VungleSDK* sdk = [VungleSDK sharedSDK];
#ifdef EE_VUNGLE_VERSION_4
    return [sdk playAd:view error:nil];
#else  // EE_VUNGLE_VERSION_4
    return [sdk playAd:view options:nil placementID:placementId error:nil];
#endif // EE_VUNGLE_VERSION_4
}

#ifdef EE_VUNGLE_VERSION_4
- (void)vungleSDKWillCloseAdWithViewInfo:(NSDictionary*)viewInfo {
    NSLog(@"%s: info = %@", __PRETTY_FUNCTION__, viewInfo);
    if ([viewInfo[@"completedView"] boolValue]) {
        NSMutableDictionary* dict = [NSMutableDictionary dictionary];
        [dict setValue:@(2) forKey:@"code"];
        [dict setValue:@"rewardVideo" forKey:@"placement"];

        EEMessageBridge* bridge = [EEMessageBridge getInstance];
        [bridge callCpp:k__Vungle_cpp_callback
                    msg:[EEJsonUtils convertDictionaryToString:dict]];
    }
}

- (void)vungleSDKAdPlayableChanged:(BOOL)isAdPlayable {
    NSLog(@"%s: %d", __PRETTY_FUNCTION__, (int)isAdPlayable);
}
#else  // EE_VUNGLE_VERSION_4
- (void)vungleWillCloseAdWithViewInfo:(nonnull VungleViewInfo*)info
                          placementID:(nonnull NSString*)placementId {
    NSLog(@"%s: info = %@ id = %@", __PRETTY_FUNCTION__, viewInfo, placementId);
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    [dict setValue:@(2) forKey:@"code"];
    [dict setValue:placementId forKey:@"placement"];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__Vungle_cpp_callback
                msg:[EEJsonUtils convertDictionaryToString:dict]];
}

- (void)vungleAdPlayabilityUpdate:(BOOL)isAdPlayable
                      placementID:(nullable NSString*)placementId {
    NSLog(@"%s: playable = %d id = %@", __PRETTY_FUNCTION__, (int)isAdPlayable,
          placementId);
}
#endif // EE_VUNGLE_VERSION_4

@end
