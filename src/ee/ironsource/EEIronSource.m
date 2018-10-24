//
//  EEUnityAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <IronSource/IronSource.h>
#import <UIKit/UIKit.h>

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/ironsource/EEIronSource.h"

@interface EEIronSource () <ISRewardedVideoDelegate, ISInterstitialDelegate> {
    BOOL initialized_;
    id<EEIMessageBridge> bridge_;
}

@end

@implementation EEIronSource

static NSString* const k__initialize        = @"IronSource_initialize";
static NSString* const k__hasRewardedVideo  = @"IronSource_hasRewardedVideo";
static NSString* const k__showRewardedVideo = @"IronSource_showRewardedVideo";

static NSString* const k__loadInterstitial  = @"IronSource_loadInterstitial";
static NSString* const k__hasInterstitial   = @"IronSource_hasInterstitial";
static NSString* const k__showInterstitial  = @"IronSource_showInterstitial";

static NSString* const k__onRewarded = @"IronSource_onRewarded";
static NSString* const k__onFailed   = @"IronSource_onFailed";
static NSString* const k__onOpened   = @"IronSource_onOpened";
static NSString* const k__onClosed   = @"IronSource_onClosed";

static NSString* const k__onInterstitialFailed  = @"IronSource_onInterstitialFailed";
static NSString* const k__onInterstitialOpened  = @"IronSource_onInterstitialOpened";
static NSString* const k__onInterstitialClosed  = @"IronSource_onInterstitialClosed";
static NSString* const k__onInterstitialClicked = @"IronSource_onInterstitialClicked";

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    initialized_ = NO;
    bridge_ = [EEMessageBridge getInstance];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [self destroy];
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:k__initialize
                    callback:^(NSString* message) {
                        NSString* gameId = message;
                        [self initialize:gameId];
                        return @"";
                    }];

    [bridge_ registerHandler:k__hasRewardedVideo
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self hasRewardedVideo]];
                    }];

    [bridge_ registerHandler:k__showRewardedVideo
                    callback:^(NSString* message) {
                        NSString* placementId = message;
                        [self showRewardedVideo:placementId];
                        return @"";
                    }];

    [bridge_ registerHandler:k__loadInterstitial
                    callback:^(NSString* message) {
                        [self loadInterstitial];
                        return @"";
                    }];

    [bridge_ registerHandler:k__hasInterstitial
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self hasInterstitial]];
                    }];

    [bridge_ registerHandler:k__showInterstitial
                    callback:^(NSString* message) {
                        NSString* placementId = message;
                        [self showInterstitial:placementId];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__hasRewardedVideo];
    [bridge_ deregisterHandler:k__showRewardedVideo];
    [bridge_ deregisterHandler:k__hasInterstitial];
    [bridge_ deregisterHandler:k__showInterstitial];
}
#pragma mark - Init
- (void)initialize:(NSString*)gameId {
    if (initialized_) {
        return;
    }
    [IronSource initWithAppKey:gameId
                       adUnits:@[IS_REWARDED_VIDEO, IS_INTERSTITIAL]];
    [IronSource setRewardedVideoDelegate:self];
    [IronSource setInterstitialDelegate:self];

    [IronSource setUserId:[IronSource advertiserId]];
    initialized_ = YES;
}

- (void)destroy {
    if (!initialized_) {
        return;
    }
    [IronSource setRewardedVideoDelegate:nil];
}
#pragma mark - Video
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
    [bridge_ callCpp:k__onRewarded message:[placementInfo placementName]];
}

- (void)rewardedVideoDidFailToShowWithError:(NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onFailed];
}

- (void)rewardedVideoDidOpen {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onOpened];
}

- (void)rewardedVideoDidClose {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onClosed];
}

- (void)rewardedVideoDidStart {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)rewardedVideoDidEnd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)didClickRewardedVideo:(ISPlacementInfo*)placementInfo {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [placementInfo placementName]);
}
#pragma mark - Interstitial
- (void)loadInterstitial {
    [IronSource loadInterstitial];
}

- (BOOL)hasInterstitial {
    return [IronSource hasInterstitial];
}

- (void)showInterstitial:(NSString* _Nonnull)placementId {
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [IronSource showInterstitialWithViewController:rootView
                                         placement:placementId];
}

- (void)interstitialDidLoad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)interstitialDidFailToLoadWithError:(NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)interstitialDidOpen {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialOpened];
}

- (void)interstitialDidClose {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialClosed];
}

- (void)interstitialDidShow {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)interstitialDidFailToShowWithError:(NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialFailed];
}

- (void)didClickInterstitial {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialClicked];
}
@end
