//
//  EEVungle.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <UIKit/UIKit.h>
#import <VungleSDK/VungleSDK.h>

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/vungle/EEVungle.h"

//#define EE_VUNGLE_VERSION_4

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
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* gameId = dict[@"gameId"];
                       NSString* placementId = dict[@"placementId"];
                       [self initialize:gameId placementId:placementId];
                       return @"";
                   }];

    [bridge registerHandler:k__hasRewardedVideo
                   callback:^(NSString* placementId) {
                       return [EEUtils
                           toString:[self hasRewardedVideo:placementId]];
                   }];

    [bridge registerHandler:k__showRewardedVideo
                   callback:^(NSString* placementId) {
                       return [EEUtils
                           toString:[self showRewardedVideo:placementId]];
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__initialize];
    [bridge deregisterHandler:k__hasRewardedVideo];
    [bridge deregisterHandler:k__showRewardedVideo];
}

- (void)initialize:(NSString*)gameId placementId:(NSString*)placementId {
    if (initialized_) {
        return;
    }
    VungleSDK* sdk = [VungleSDK sharedSDK];

#ifdef EE_VUNGLE_VERSION_4
    [sdk startWithAppId:gameId];
#else  // EE_VUNGLE_VERSION_4
    [sdk startWithAppId:gameId
             placements:[NSArray arrayWithObjects:placementId, nil]
                  error:nil];
#endif // placementIds

    [sdk setDelegate:self];
    initialized_ = YES;
}

- (void)destroy {
    VungleSDK* sdk = [VungleSDK sharedSDK];
    [sdk setDelegate:nil];
}

- (BOOL)hasRewardedVideo:(NSString* _Nonnull)placementId {
    VungleSDK* sdk = [VungleSDK sharedSDK];
#ifdef EE_VUNGLE_VERSION_4
    return [sdk isAdPlayable];
#else  // EE_VUNGLE_VERSION_4
    return [sdk isAdCachedForPlacementID:placementId];
#endif // EE_VUNGLE_VERSION_4
}

- (BOOL)showRewardedVideo:(NSString* _Nonnull)placementId {
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
- (void)vungleWillShowAdForPlacementID:(nullable NSString*)placementID {
    NSLog(@"%s %@", __PRETTY_FUNCTION__, placementID);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onStart];
}

- (void)vungleWillCloseAdWithViewInfo:(VungleViewInfo*)info
                          placementID:(NSString*)placementID {
    BOOL result = [info.completedView boolValue];
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onEnd message:[EEUtils toString:result]];
}

- (void)vungleAdPlayabilityUpdate:(BOOL)isAdPlayable
                      placementID:(NSString*)placementID {
    NSLog(@"%s: playable = %d id = %@", __PRETTY_FUNCTION__, (int)isAdPlayable,
          placementID);
}
#endif // EE_VUNGLE_VERSION_4

@end
