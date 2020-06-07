//
//  EEFacebookInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import "ee/facebook_ads/private/EEFacebookRewardedAd.h"

#import <FBAudienceNetwork/FBRewardedVideoAd.h>

#import <ee_x-Swift.h>

#import <ee/ads/internal/EEMessageHelper.h>
#import <ee/core/internal/EEJsonUtils.h>

@interface EEFacebookRewardedAd () <FBRewardedVideoAdDelegate>
@end

@implementation EEFacebookRewardedAd {
    id<EEIMessageBridge> bridge_;
    NSString* adId_;
    EEMessageHelper* messageHelper_;
    BOOL rewarded_;
    FBRewardedVideoAd* ad_;
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
        [[EEMessageHelper alloc] initWithPrefix:@"FacebookRewardedAd"
                                           adId:adId];
    [self createInternalAd];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [super dealloc];
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
    ad_ = [[FBRewardedVideoAd alloc] initWithPlacementID:adId_];
    [ad_ setDelegate:self];
    return YES;
}

- (BOOL)destroyInternalAd {
    NSAssert([EEUtils isMainThread], @"");
    if (ad_ == nil) {
        return NO;
    }
    [ad_ setDelegate:nil];
    [ad_ release];
    ad_ = nil;
    return YES;
}

- (BOOL)isLoaded {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    return [ad_ isAdValid];
}

- (void)load {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    [ad_ loadAd];
}

- (void)show {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    rewarded_ = NO;
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    BOOL result = [ad_ showAdFromRootViewController:rootView];
    if (result) {
        // OK.
    } else {
        [bridge_ callCpp:[messageHelper_ onFailedToShow]];
    }
}

- (void)rewardedVideoAdDidClick:(FBRewardedVideoAd*)rewardedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedVideoAd, @"");
    [bridge_ callCpp:[messageHelper_ onClicked]];
}

- (void)rewardedVideoAdDidLoad:(FBRewardedVideoAd*)rewardedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedVideoAd, @"");
    [bridge_ callCpp:[messageHelper_ onLoaded]];
}

- (void)rewardedVideoAdDidClose:(FBRewardedVideoAd*)rewardedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedVideoAd, @"");
    [bridge_ callCpp:[messageHelper_ onClosed]
             message:[EEUtils toString:rewarded_]];
}

- (void)rewardedVideoAdWillClose:(FBRewardedVideoAd*)rewardedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedVideoAd, @"");
}

- (void)rewardedVideoAd:(FBRewardedVideoAd*)rewardedVideoAd
       didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(ad_ == rewardedVideoAd, @"");
    [bridge_ callCpp:[messageHelper_ onFailedToLoad]
             message:[error description]];
}

- (void)rewardedVideoAdVideoComplete:(FBRewardedVideoAd*)rewardedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedVideoAd, @"");
    rewarded_ = YES;
}

- (void)rewardedVideoAdWillLogImpression:(FBRewardedVideoAd*)rewardedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedVideoAd, @"");
}

- (void)rewardedVideoAdServerRewardDidSucceed:
    (FBRewardedVideoAd*)rewardedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedVideoAd, @"");
}

- (void)rewardedVideoAdServerRewardDidFail:(FBRewardedVideoAd*)rewardedVideoAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == rewardedVideoAd, @"");
}

@end
