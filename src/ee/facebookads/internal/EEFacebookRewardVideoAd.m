//
//  EEFacebookInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import <FBAudienceNetwork/FBAudienceNetwork.h>

#import "ee/ads/internal/EEInterstitialAdHelper.h"
#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebookads/internal/EEFacebookRewardVideoAd.h"

@interface EEFacebookRewardVideoAd () <FBRewardedVideoAdDelegate> {
    NSString* placementId_;
    FBRewardedVideoAd* rewardedVideoAd_;
}

@end

@implementation EEFacebookRewardVideoAd

- (NSString*)k__hasRewardedVideo {
    return
        [@"FacebookAds_hasRewardedVideo_" stringByAppendingString:placementId_];
}

- (NSString*)k__loadRewardedVideo {
    return [@"FacebookAds_loadRewardedVideo_"
        stringByAppendingString:placementId_];
}

- (NSString*)k__showRewardedVideo {
    return [@"FacebookAds_showRewardedVideo_"
        stringByAppendingString:placementId_];
}

- (NSString*)k__onRewarded {
    return
        [@"FacebookAds_Video_onRewarded_" stringByAppendingString:placementId_];
}

- (NSString*)k__onFailedToLoad {
    return [@"FacebookAds_Video_onFailedToLoad_"
        stringByAppendingString:placementId_];
}

- (NSString*)k__onLoaded {
    return
        [@"FacebookAds_Video_onLoaded_" stringByAppendingString:placementId_];
}

- (NSString*)k__onOpened {
    return
        [@"FacebookAds_Video_onOpened_" stringByAppendingString:placementId_];
}

- (NSString*)k__onClosed {
    return
        [@"FacebookAds_Video_onClosed_" stringByAppendingString:placementId_];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__hasRewardedVideo]
                   callback:^(NSString* message) {
                       return [EEUtils toString:[self hasRewardVideo]];
                   }];

    [bridge registerHandler:[self k__loadRewardedVideo]
                   callback:^(NSString* message) {
                       [self loadRewardedVideo];
                       return @"";
                   }];

    [bridge registerHandler:[self k__showRewardedVideo]
                   callback:^(NSString* message) {
                       [self showRewardVideo];
                       return @"";
                   }];
}
- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__hasRewardedVideo]];
    [bridge deregisterHandler:[self k__loadRewardedVideo]];
    [bridge deregisterHandler:[self k__showRewardedVideo]];
}

- (id _Nonnull)initWithPlacementId:(NSString* _Nonnull)placementId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    placementId_ = [placementId copy];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
}

- (void)destroy {
    [rewardedVideoAd_ dealloc];
}

- (BOOL)k_createVideoAd {
    rewardedVideoAd_ =
        [[FBRewardedVideoAd alloc] initWithPlacementID:placementId_];
    rewardedVideoAd_.delegate = self;
    [rewardedVideoAd_ loadAd];

    return YES;
}

#pragma mark - video methods
- (BOOL)hasRewardVideo {
    return [rewardedVideoAd_ isAdValid];
}

- (void)loadRewardedVideo {
    [rewardedVideoAd_ loadAd];
}

- (BOOL)showRewardVideo {
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    return [rewardedVideoAd_ showAdFromRootViewController:rootView];
}

#pragma mark - facebook delegate
- (void)rewardedVideoAdDidClick:(FBRewardedVideoAd*)rewardedVideoAd {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onOpened]];
}

- (void)rewardedVideoAdDidLoad:(FBRewardedVideoAd*)rewardedVideoAd {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onLoaded]];
}

- (void)rewardedVideoAdDidClose:(FBRewardedVideoAd*)rewardedVideoAd {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onClosed]];
}

- (void)rewardedVideoAdWillClose:(FBRewardedVideoAd*)rewardedVideoAd {
}

- (void)rewardedVideoAd:(FBRewardedVideoAd*)rewardedVideoAd
       didFailWithError:(NSError*)error {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onFailedToLoad]];
}

- (void)rewardedVideoAdComplete:(FBRewardedVideoAd*)rewardedVideoAd {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onRewarded]];
}

- (void)rewardedVideoAdWillLogImpression:(FBRewardedVideoAd*)rewardedVideoAd {
}

- (void)rewardedVideoAdServerRewardDidSucceed:
    (FBRewardedVideoAd*)rewardedVideoAd {
}

- (void)rewardedVideoAdServerRewardDidFail:(FBRewardedVideoAd*)rewardedVideoAd {
}

@end
