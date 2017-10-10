//
//  EEUnityAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <IronSource/IronSource.h>
#import <UIKit/UIKit.h>

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/ironsource/EEIronSource.h"

@interface EEIronSource () <ISRewardedVideoDelegate>
@end

@implementation EEIronSource

// clang-format off
static NSString* const k__initialize        = @"IronSource_initialize";
static NSString* const k__hasRewardedVideo  = @"IronSource_hasRewardedVideo";
static NSString* const k__showRewardedVideo = @"IronSource_showRewardedVideo";
static NSString* const k__onRewarded        = @"IronSource_onRewarded";
static NSString* const k__onFailed          = @"IronSource_onFailed";
static NSString* const k__onOpened          = @"IronSource_onOpened";
static NSString* const k__onClosed          = @"IronSource_onClosed";
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
    [IronSource setRewardedVideoDelegate:nil];
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
                       return [self hasRewardedVideo] ? @"true" : @"false";
                   }];

    [bridge registerHandler:k__showRewardedVideo
                   callback:^(NSString* message) {
                       NSString* placementId = message;
                       [self showRewardedVideo:placementId];
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
    [IronSource initWithAppKey:gameId adUnits:@[IS_REWARDED_VIDEO]];
    [IronSource setRewardedVideoDelegate:self];
}

- (BOOL)hasRewardedVideo {
    return [IronSource hasRewardedVideo];
}

- (void)showRewardedVideo:(NSString* _Nonnull)placementId {
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [IronSource showRewardedVideoWithViewController:rootView
                                          placement:placementId];
}

- (void)rewardedVideoHasChangedAvailability:(BOOL)available {
    NSLog(@"%s: %d", __PRETTY_FUNCTION__, (int)available);
}

- (void)didReceiveRewardForPlacement:(ISPlacementInfo*)placementInfo {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [placementInfo placementName]);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onRewarded message:[placementInfo placementName]];
}

- (void)rewardedVideoDidFailToShowWithError:(NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onFailed];
}

- (void)rewardedVideoDidOpen {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onOpened];
}

- (void)rewardedVideoDidClose {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__onClosed];
}

- (void)rewardedVideoDidStart {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)rewardedVideoDidEnd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
