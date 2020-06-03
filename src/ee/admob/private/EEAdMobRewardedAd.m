//
//  EEAdMobRewardedVideo.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/admob/private/EEAdMobRewardedAd.h"

#import <GoogleMobileAds/GADRewardedAd.h>

#import <ee_x-Swift.h>

#import <ee/ads/internal/EEMessageHelper.h>
#import <ee/core/internal/EEIMessageBridge.h>
#import <ee/core/internal/EEJsonUtils.h>

@interface EEAdMobRewardedAd () <GADRewardedAdDelegate>
@end

@implementation EEAdMobRewardedAd {
    id<EEIMessageBridge> bridge_;
    NSString* adId_;
    EEMessageHelper* messageHelper_;
    BOOL rewarded_;
    GADRewardedAd* ad_;
}

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    adId_ = [adId copy];
    messageHelper_ =
        [[EEMessageHelper alloc] initWithPrefix:@"AdMobRewardedAd" adId:adId];
    [self createInternalAd];
    [self registerHandlers];
    return self;
}

- (void)destroy {
    NSAssert([EEUtils isMainThread], @"");
    [self deregisterHandlers];
    [self destroyInternalAd];

    [adId_ release];
    adId_ = nil;
    [messageHelper_ release];
    messageHelper_ = nil;
}

- (void)dealloc {
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:[messageHelper_ createInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self createInternalAd]];
                    }];
    [bridge_ registerHandler:[messageHelper_ destroyInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self destroyInternalAd]];
                    }];
    [bridge_ registerHandler:[messageHelper_ isLoaded]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self isLoaded]];
                    }];
    [bridge_ registerHandler:[messageHelper_ load]
                    callback:^(NSString* message) {
                        [self load];
                        return @"";
                    }];
    [bridge_ registerHandler:[messageHelper_ show]
                    callback:^(NSString* message) {
                        [self show];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:[messageHelper_ createInternalAd]];
    [bridge_ deregisterHandler:[messageHelper_ destroyInternalAd]];
    [bridge_ deregisterHandler:[messageHelper_ isLoaded]];
    [bridge_ deregisterHandler:[messageHelper_ load]];
    [bridge_ deregisterHandler:[messageHelper_ show]];
}

- (BOOL)createInternalAd {
    NSAssert([EEUtils isMainThread], @"");
    if (ad_ != nil) {
        return NO;
    }
    ad_ = [[GADRewardedAd alloc] initWithAdUnitID:adId_];
    return YES;
}

- (BOOL)destroyInternalAd {
    NSAssert([EEUtils isMainThread], @"");
    if (ad_ == nil) {
        return NO;
    }
    [ad_ release];
    ad_ = nil;
    return YES;
}

- (BOOL)isLoaded {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    return [ad_ isReady];
}

- (void)load {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    GADRequest* request = [GADRequest request];
    [ad_ loadRequest:request
        completionHandler:^(GADRequestError* _Nullable error) {
            if (error) {
                [bridge_ callCpp:[messageHelper_ onFailedToLoad]
                         message:[error description]];
            } else {
                [bridge_ callCpp:[messageHelper_ onLoaded]];
            }
        }];
}

- (void)show {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    rewarded_ = NO;
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [ad_ presentFromRootViewController:rootView delegate:self];
}

/// Tells the delegate that the user earned a reward.
- (void)rewardedAd:(nonnull GADRewardedAd*)rewardedAd
    userDidEarnReward:(nonnull GADAdReward*)reward {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedAd, @"");
    rewarded_ = YES;
}

/// Tells the delegate that the rewarded ad failed to present.
- (void)rewardedAd:(nonnull GADRewardedAd*)rewardedAd
    didFailToPresentWithError:(nonnull NSError*)error {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedAd, @"");
    [bridge_ callCpp:[messageHelper_ onFailedToShow]
             message:[error description]];
}

/// Tells the delegate that the rewarded ad was presented.
- (void)rewardedAdDidPresent:(nonnull GADRewardedAd*)rewardedAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedAd, @"");
}

/// Tells the delegate that the rewarded ad was dismissed.
- (void)rewardedAdDidDismiss:(nonnull GADRewardedAd*)rewardedAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedAd, @"");
    [bridge_ callCpp:[messageHelper_ onClosed]
             message:[EEUtils toString:rewarded_]];
}

@end
