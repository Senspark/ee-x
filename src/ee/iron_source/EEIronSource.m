//
//  EEUnityAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/iron_source/EEIronSource.h"

#import <UIKit/UIKit.h>

#import <IronSource/IronSource.h>

#import <ee_x-Swift.h>

#import <ee/core/internal/EEJsonUtils.h>

#define kPrefix @"IronSource"

// clang-format off
static NSString* const k__initialize         = kPrefix "_initialize";

static NSString* const k__loadInterstitialAd = kPrefix "_loadInterstitialAd";
static NSString* const k__hasInterstitialAd  = kPrefix "_hasInterstitialAd";
static NSString* const k__showInterstitialAd = kPrefix "_showInterstitialAd";

static NSString* const k__hasRewardedAd      = kPrefix "_hasRewardedAd";
static NSString* const k__showRewardedAd     = kPrefix "_showRewardedAd";

static NSString* const k__onInterstitialAdLoaded       = kPrefix "_onInterstitialAdLoaded";
static NSString* const k__onInterstitialAdFailedToLoad = kPrefix "_onInterstitialAdFailedToLoad";
static NSString* const k__onInterstitialAdFailedToShow = kPrefix "_onInterstitialAdFailedToShow";
static NSString* const k__onInterstitialAdClicked      = kPrefix "_onInterstitialAdClicked";
static NSString* const k__onInterstitialAdClosed       = kPrefix "_onInterstitialAdClosed";

static NSString* const k__onRewardedAdLoaded       = kPrefix "_onRewardedAdLoaded";
static NSString* const k__onRewardedAdFailedToShow = kPrefix "_onRewardedAdFailedToShow";
static NSString* const k__onRewardedAdClicked      = kPrefix "_onRewardedAdClicked";
static NSString* const k__onRewardedAdClosed       = kPrefix "_onRewardedAdClosed";
// clang-format on

#undef kPrefix

@interface EEIronSource () <ISRewardedVideoDelegate, ISInterstitialDelegate>
@end

@implementation EEIronSource {
    id<EEIMessageBridge> bridge_;
    BOOL initialized_;
    BOOL rewarded_;
}

- (id)init {
    NSAssert([EEThread isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = [EEMessageBridge getInstance];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)destroy {
    NSAssert([EEThread isMainThread], @"");
    [self deregisterHandlers];
    if (!initialized_) {
        return;
    }
    [IronSource setInterstitialDelegate:nil];
    [IronSource setRewardedVideoDelegate:nil];
}

- (void)registerHandlers {
    [bridge_ registerHandler:
               k__initialize:^(NSString* message) {
                   NSString* gameId = message;
                   [self initialize:gameId];
                   return @"";
               }];

    [bridge_ registerHandler:
        k__hasInterstitialAd:^(NSString* message) {
            return [EEUtils toString:[self hasInterstitialAd]];
        }];

    [bridge_ registerHandler:
        k__loadInterstitialAd:^(NSString* message) {
            [self loadInterstitialAd];
            return @"";
        }];

    [bridge_ registerHandler:
        k__showInterstitialAd:^(NSString* message) {
            NSString* adId = message;
            [self showInterstitialAd:adId];
            return @"";
        }];

    [bridge_ registerHandler:
            k__hasRewardedAd:^(NSString* message) {
                return [EEUtils toString:[self hasRewardedAd]];
            }];

    [bridge_ registerHandler:
           k__showRewardedAd:^(NSString* message) {
               NSString* adId = message;
               [self showRewardedAd:adId];
               return @"";
           }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__hasInterstitialAd];
    [bridge_ deregisterHandler:k__showInterstitialAd];
    [bridge_ deregisterHandler:k__hasRewardedAd];
    [bridge_ deregisterHandler:k__showRewardedAd];
}

#pragma mark - Initialization

- (void)initialize:(NSString*)gameId {
    NSAssert([EEThread isMainThread], @"");
    if (initialized_) {
        return;
    }
    [IronSource initWithAppKey:gameId
                       adUnits:@[IS_REWARDED_VIDEO, IS_INTERSTITIAL]];
    [IronSource shouldTrackReachability:YES];
    [IronSource setInterstitialDelegate:self];
    [IronSource setRewardedVideoDelegate:self];
    [IronSource setUserId:[IronSource advertiserId]];
    initialized_ = YES;
}

#pragma mark - Interstitial Ad

- (BOOL)hasInterstitialAd {
    NSAssert([EEThread isMainThread], @"");
    return [IronSource hasInterstitial];
}

- (void)loadInterstitialAd {
    NSAssert([EEThread isMainThread], @"");
    [IronSource loadInterstitial];
}

- (void)showInterstitialAd:(NSString* _Nonnull)adId {
    NSAssert([EEThread isMainThread], @"");
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [IronSource showInterstitialWithViewController:rootView placement:adId];
}

- (void)interstitialDidLoad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdLoaded];
}

- (void)interstitialDidFailToLoadWithError:(NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdFailedToLoad //
                    :[error localizedDescription]];  // [error description]
                                                     // returns empty.
}

- (void)interstitialDidOpen {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)interstitialDidClose {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdClosed];
}

- (void)interstitialDidShow {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)interstitialDidFailToShowWithError:(NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdFailedToShow //
                    :[error description]];
}

- (void)didClickInterstitial {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdClicked];
}

#pragma mark - Rewarded Ad

- (BOOL)hasRewardedAd {
    NSAssert([EEThread isMainThread], @"");
    return [IronSource hasRewardedVideo];
}

- (void)showRewardedAd:(NSString* _Nonnull)adId {
    NSAssert([EEThread isMainThread], @"");
    rewarded_ = NO;
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [IronSource showRewardedVideoWithViewController:rootView placement:adId];
}

- (void)handleRewardedAdResult {
    [bridge_ callCpp:k__onRewardedAdClosed //
                    :[EEUtils toString:rewarded_]];
}

- (void)rewardedVideoHasChangedAvailability:(BOOL)available {
    NSLog(@"%s: %d", __PRETTY_FUNCTION__, (int)available);
    if (available) {
        [bridge_ callCpp:k__onRewardedAdLoaded];
    }
}

- (void)didReceiveRewardForPlacement:(ISPlacementInfo*)placementInfo {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [placementInfo placementName]);
    rewarded_ = YES;
}

- (void)rewardedVideoDidFailToShowWithError:(NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onRewardedAdFailedToShow //
                    :[error description]];
}

- (void)rewardedVideoDidOpen {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)rewardedVideoDidClose {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    // Note: The didReceiveReward and rewardedVideoDidClose events are
    // asynchronous.
    if (rewarded_) {
        // Already has result.
        [self handleRewardedAdResult];
    } else {
        [EEThread runOnMainThreadDelayed:
                                    1.0f:^{
                                        [self handleRewardedAdResult];
                                    }];
    }
}

- (void)rewardedVideoDidStart {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)rewardedVideoDidEnd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)didClickRewardedVideo:(ISPlacementInfo*)placementInfo {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [placementInfo placementName]);
    [bridge_ callCpp:k__onRewardedAdClicked];
}

@end
