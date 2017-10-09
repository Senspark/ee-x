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
static NSString* const k__initialize        = @"Vungle_initialize";
static NSString* const k__showRewardedVideo = @"Vungle_showRewardedVideo";
static NSString* const k__cppCallback       = @"Vungle_cppCallback";
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

    [bridge registerHandler:k__initialize
                   callback:^(NSString* message) {
                       NSString* gameId = message;
                       [self initialize:gameId];
                       return @"";
                   }];

    [bridge registerHandler:k__showRewardedVideo
                   callback:^(NSString* message) {
                       return [self showRewardedVideo] ? @"true" : @"false";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__initialize];
    [bridge deregisterHandler:k__showRewardedVideo];
}

- (void)initialize:(NSString*)gameId {
    VungleSDK* sdk = [VungleSDK sharedSDK];

#ifdef EE_VUNGLE_VERSION_4
    [sdk startWithAppId:gameId];
#else  // EE_VUNGLE_VERSION_4
    [sdk startWithAppId:gameId placements:placementIds error:nil];
#endif // placementIds

    [sdk setLoggingEnabled:YES];
    [sdk setDelegate:self];
}

- (BOOL)isRewardedVideoReady:(NSString*)placementId {
    VungleSDK* sdk = [VungleSDK sharedSDK];
#ifdef EE_VUNGLE_VERSION_4
    return [sdk isAdPlayable];
#else  // EE_VUNGLE_VERSION_4
    return [sdk isAdCachedForPlacementID:placementId];
#endif // EE_VUNGLE_VERSION_4
}

- (BOOL)showRewardedVideo:(NSString*)placementId {
    if (![self isRewardedVideoReady:placementId]) {
        return NO;
    }
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
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    BOOL result = [viewInfo[@"completedView"] boolValue];
    [dict setValue:@(result) forKey:@"result"];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__cppCallback
            message:[EEJsonUtils convertDictionaryToString:dict]];
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
    [bridge callCpp:k__cppCallback
            message:[EEJsonUtils convertDictionaryToString:dict]];
}

- (void)vungleAdPlayabilityUpdate:(BOOL)isAdPlayable
                      placementID:(nullable NSString*)placementId {
    NSLog(@"%s: playable = %d id = %@", __PRETTY_FUNCTION__, (int)isAdPlayable,
          placementId);
}
#endif // EE_VUNGLE_VERSION_4

@end
