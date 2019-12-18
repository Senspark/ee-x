//
//  EEAdMobRewardedVideo.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <GoogleMobileAds/GoogleMobileAds.h>

#import "ee/admob/internal/EEAdMobRewardedVideo.h"
#import "ee/core/EEIMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"

@interface EEAdMobRewardedVideo () <GADRewardedAdDelegate> {
    id<EEIMessageBridge> bridge_;
    NSString* adId_;
    GADRewardedAd* rewardedAd_;
}

@end

@implementation EEAdMobRewardedVideo

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    adId_ = [adId copy];
    rewardedAd_ = nil;

    [self registerHandlers];
    return self;
}

- (void)destroy {
    [self deregisterHandlers];
}

- (void)dealloc {
    [adId_ release];
    adId_ = nil;
    [super dealloc];
}

- (NSString*)k__createInternalAd {
    return
        [@"AdMobRewardVideoAd_createInternalAd_" stringByAppendingString:adId_];
}

- (NSString*)k__destroyInternalAd {
    return [@"AdMobRewardVideoAd_destroyInternalAd_"
        stringByAppendingString:adId_];
}

- (NSString*)k__isLoaded {
    return [@"AdMobRewardVideoAd_isLoaded_" stringByAppendingString:adId_];
}

- (NSString*)k__onLoaded {
    return [@"AdMobRewardVideoAd_onLoaded_" stringByAppendingString:adId_];
}

- (NSString*)k__load {
    return [@"AdMobRewardVideoAd_load_" stringByAppendingString:adId_];
}

- (NSString*)k__show {
    return [@"AdMobRewardVideoAd_show_" stringByAppendingString:adId_];
}

- (NSString*)k__onFailedToLoad {
    return
        [@"AdMobRewardVideoAd_onFailedToLoad_" stringByAppendingString:adId_];
}

- (NSString*)k__onFailedToShow {
    return
        [@"AdMobRewardVideoAd_onFailedToShow_" stringByAppendingString:adId_];
}

- (NSString* _Nonnull)k__onReward {
    return [@"AdMobRewardVideoAd_onReward_" stringByAppendingString:adId_];
}

- (NSString*)k__onClosed {
    return [@"AdMobRewardVideoAd_onClosed_" stringByAppendingString:adId_];
}

- (void)registerHandlers {
    [bridge_ registerHandler:[self k__createInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self createInternalAd]];
                    }];

    [bridge_ registerHandler:[self k__destroyInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self destroyInternalAd]];
                    }];
    [bridge_ registerHandler:[self k__isLoaded]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self isLoaded]];
                    }];
    [bridge_ registerHandler:[self k__load]
                    callback:^(NSString* message) {
                        [self load];
                        return @"";
                    }];
    [bridge_ registerHandler:[self k__show]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self show]];
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:[self k__createInternalAd]];
    [bridge_ deregisterHandler:[self k__destroyInternalAd]];
    [bridge_ deregisterHandler:[self k__isLoaded]];
    [bridge_ deregisterHandler:[self k__load]];
    [bridge_ deregisterHandler:[self k__show]];
}

- (BOOL)createInternalAd {
    if (rewardedAd_ != nil) {
        return NO;
    }
    GADRewardedAd* rewadedAd =
        [[[GADRewardedAd alloc] initWithAdUnitID:adId_] autorelease];
    rewardedAd_ = [rewadedAd retain];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (rewardedAd_ == nil) {
        return NO;
    }
    [rewardedAd_ release];
    rewardedAd_ = nil;
    return YES;
}

- (BOOL)isLoaded {
    if (rewardedAd_ == nil) {
        return NO;
    }
    return [rewardedAd_ isReady];
}

- (void)load {
    if (rewardedAd_ == nil) {
        return;
    }
    GADRequest* request = [GADRequest request];
    [rewardedAd_ loadRequest:request
           completionHandler:^(GADRequestError* _Nullable error) {
               if (error) {
                   [bridge_ callCpp:[self k__onFailedToLoad]
                            message:[error description]];
               } else {
                   [bridge_ callCpp:[self k__onLoaded]];
               }
           }];
}

- (BOOL)show {
    if (rewardedAd_ == nil) {
        return NO;
    } else if (![rewardedAd_ isReady]) {
        return NO;
    }

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [rewardedAd_ presentFromRootViewController:rootView delegate:self];
    return YES;
}

/// Tells the delegate that the user earned a reward.
- (void)rewardedAd:(nonnull GADRewardedAd*)rewardedAd
    userDidEarnReward:(nonnull GADAdReward*)reward {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:[self k__onReward]];
}

/// Tells the delegate that the rewarded ad failed to present.
- (void)rewardedAd:(nonnull GADRewardedAd*)rewardedAd
    didFailToPresentWithError:(nonnull NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:[self k__onFailedToShow]];
}

/// Tells the delegate that the rewarded ad was presented.
- (void)rewardedAdDidPresent:(nonnull GADRewardedAd*)rewardedAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

/// Tells the delegate that the rewarded ad was dismissed.
- (void)rewardedAdDidDismiss:(nonnull GADRewardedAd*)rewardedAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:[self k__onClosed]];
}

@end
