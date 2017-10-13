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

@interface EEVungle () <VungleSDKDelegate> {
    BOOL initialized_;
}

@end

@implementation EEVungle

// clang-format off
static NSString* const k__initialize        = @"Vungle_initialize";
static NSString* const k__hasRewardedVideo  = @"Vungle_hasRewardedVideo";
static NSString* const k__showRewardedVideo = @"Vungle_showRewardedVideo";
static NSString* const k__onStart           = @"Vungle_onStart";
static NSString* const k__onEnd             = @"Vungle_onEnd";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    initialized_ = NO;
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [self destroy];
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

    [bridge registerHandler:k__hasRewardedVideo
                   callback:^(NSString* message) {
                       return [EEUtils toString:[self hasRewardedVideo]];
                   }];

    [bridge registerHandler:k__showRewardedVideo
                   callback:^(NSString* message) {
                       [self showRewardedVideo];
                       return @"";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__initialize];
    [bridge deregisterHandler:k__hasRewardedVideo];
    [bridge deregisterHandler:k__showRewardedVideo];
}

- (void)initialize:(NSString*)gameId {
    if (initialized_) {
        return;
    }
    VungleSDK* sdk = [VungleSDK sharedSDK];

#ifdef EE_VUNGLE_VERSION_4
    [sdk startWithAppId:gameId];
#else  // EE_VUNGLE_VERSION_4
    [sdk startWithAppId:gameId placements:placementIds error:nil];
#endif // placementIds

    [sdk setDelegate:self];
    initialized_ = YES;
}

- (void)destroy {
    VungleSDK* sdk = [VungleSDK sharedSDK];
    [sdk setDelegate:nil];
}

- (BOOL)hasRewardedVideo {
    VungleSDK* sdk = [VungleSDK sharedSDK];
#ifdef EE_VUNGLE_VERSION_4
    return [sdk isAdPlayable];
#else  // EE_VUNGLE_VERSION_4
    return [sdk isAdCachedForPlacementID:placementId];
#endif // EE_VUNGLE_VERSION_4
}

- (BOOL)showRewardedVideo {
    UIViewController* view = [EEUtils getCurrentRootViewController];
    VungleSDK* sdk = [VungleSDK sharedSDK];
#ifdef EE_VUNGLE_VERSION_4
    return [sdk playAd:view error:nil];
#else  // EE_VUNGLE_VERSION_4
    return [sdk playAd:view options:nil placementID:placementId error:nil];
#endif // EE_VUNGLE_VERSION_4
}

#ifdef EE_VUNGLE_VERSION_4
- (void)vungleSDKwillShowAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onStart];
}

- (void)vungleSDKWillCloseAdWithViewInfo:(NSDictionary*)viewInfo {
    NSLog(@"%s: info = %@", __PRETTY_FUNCTION__, viewInfo);
    BOOL result = [viewInfo[@"completedView"] boolValue];
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onEnd message:[EEUtils toString:result]];
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
