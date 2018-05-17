//
//  EEFacebookInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import "ee/ads/internal/EEInterstitialAdHelper.h"
#import "ee/core/EEIMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebookads/internal/EEFacebookRewardVideoAd.h"

#ifdef EE_X_USE_IRON_SOURCE_MEDIATION
#import <ISFacebookAdapter/FBRewardedVideoAd.h>
#else // EE_X_USE_IRON_SOURCE_MEDIATION
#import <FBAudienceNetwork/FBRewardedVideoAd.h>
#endif // EE_X_USE_IRON_SOURCE_MEDIATION

@interface EEFacebookRewardVideoAd () <FBRewardedVideoAdDelegate>
@end

@implementation EEFacebookRewardVideoAd {
    id<EEIMessageBridge> bridge_;
    NSString* placementId_;
    FBRewardedVideoAd* rewardedVideoAd_;
}

- (NSString*)k__createInternalVideo {
    return [@"FacebookAds_createInternalVideo_"
        stringByAppendingString:placementId_];
}

- (NSString*)k__destroyInternalVideo {
    return [@"FacebookAds_destroyInternalVideo_"
        stringByAppendingString:placementId_];
}

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
    [bridge_ registerHandler:[self k__createInternalVideo]
                    callback:^(NSString* message) {
                        [self createInternalVideo];
                        return @"";
                    }];

    [bridge_ registerHandler:[self k__destroyInternalVideo]
                    callback:^(NSString* message) {
                        [self destroyInternalVideo];
                        return @"";
                    }];

    [bridge_ registerHandler:[self k__hasRewardedVideo]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self hasRewardVideo]];
                    }];

    [bridge_ registerHandler:[self k__loadRewardedVideo]
                    callback:^(NSString* message) {
                        [self loadRewardedVideo];
                        return @"";
                    }];

    [bridge_ registerHandler:[self k__showRewardedVideo]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self showRewardVideo]];
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:[self k__hasRewardedVideo]];
    [bridge_ deregisterHandler:[self k__loadRewardedVideo]];
    [bridge_ deregisterHandler:[self k__showRewardedVideo]];

    [bridge_ deregisterHandler:[self k__createInternalVideo]];
    [bridge_ deregisterHandler:[self k__destroyInternalVideo]];
}

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                  placementId:(NSString* _Nonnull)placementId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    placementId_ = [placementId copy];
    [self createInternalVideo];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [placementId_ release];
    placementId_ = nil;
    [super dealloc];
}

- (void)destroy {
    [self deregisterHandlers];
    [self destroyInternalVideo];
}

#pragma mark - video methods
- (void)createInternalVideo {
    if (rewardedVideoAd_ != nil) {
        return;
    }
    rewardedVideoAd_ =
        [[FBRewardedVideoAd alloc] initWithPlacementID:placementId_];
    [rewardedVideoAd_ setDelegate:self];
}

- (void)destroyInternalVideo {
    if (rewardedVideoAd_ == nil) {
        return;
    }
    [rewardedVideoAd_ setDelegate:nil];
    [rewardedVideoAd_ release];
    rewardedVideoAd_ = nil;
}

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
    [bridge_ callCpp:[self k__onOpened]];
}

- (void)rewardedVideoAdDidLoad:(FBRewardedVideoAd*)rewardedVideoAd {
    [bridge_ callCpp:[self k__onLoaded]];
}

- (void)rewardedVideoAdDidClose:(FBRewardedVideoAd*)rewardedVideoAd {
    [bridge_ callCpp:[self k__onClosed]];
}

- (void)rewardedVideoAdWillClose:(FBRewardedVideoAd*)rewardedVideoAd {
}

- (void)rewardedVideoAd:(FBRewardedVideoAd*)rewardedVideoAd
       didFailWithError:(NSError*)error {
    [bridge_ callCpp:[self k__onFailedToLoad]];
}

- (void)rewardedVideoAdComplete:(FBRewardedVideoAd*)rewardedVideoAd {
    [bridge_ callCpp:[self k__onRewarded]];
}

- (void)rewardedVideoAdWillLogImpression:(FBRewardedVideoAd*)rewardedVideoAd {
}

- (void)rewardedVideoAdServerRewardDidSucceed:
    (FBRewardedVideoAd*)rewardedVideoAd {
}

- (void)rewardedVideoAdServerRewardDidFail:(FBRewardedVideoAd*)rewardedVideoAd {
}

@end
