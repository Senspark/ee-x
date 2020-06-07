//
//  EEFacebookInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import "ee/facebook_ads/private/EEFacebookInterstitialAd.h"

#import <FBAudienceNetwork/FBInterstitialAd.h>

#import <ee_x-Swift.h>

#import <ee/ads/internal/EEInterstitialAdHelper.h>
#import <ee/ads/internal/EEMessageHelper.h>
#import <ee/core/internal/EEJsonUtils.h>

@interface EEFacebookInterstitialAd () <FBInterstitialAdDelegate>
@end

@implementation EEFacebookInterstitialAd {
    id<EEIMessageBridge> bridge_;
    NSString* adId_;
    EEMessageHelper* messageHelper_;
    EEInterstitialAdHelper* helper_;
    FBInterstitialAd* ad_;
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
        [[EEMessageHelper alloc] initWithPrefix:@"FacebookInterstitialAd"
                                           adId:adId];
    helper_ = [[EEInterstitialAdHelper alloc] initWithBridge:bridge_
                                                          ad:self
                                                      helper:messageHelper_];
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
    [helper_ release];
    helper_ = nil;
}

- (void)dealloc {
    [super dealloc];
}

- (void)registerHandlers {
    [helper_ registerHandlers];
    [bridge_ registerHandler:[messageHelper_ createInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self createInternalAd]];
                    }];
    [bridge_ registerHandler:[messageHelper_ destroyInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self destroyInternalAd]];
                    }];
}

- (void)deregisterHandlers {
    [helper_ deregisterHandlers];
    [bridge_ deregisterHandler:[messageHelper_ createInternalAd]];
    [bridge_ deregisterHandler:[messageHelper_ destroyInternalAd]];
}

- (BOOL)createInternalAd {
    NSAssert([EEUtils isMainThread], @"");
    if (ad_ != nil) {
        return NO;
    }
    ad_ = [[FBInterstitialAd alloc] initWithPlacementID:adId_];
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
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    BOOL result = [ad_ showAdFromRootViewController:rootView];
    if (result) {
        // OK.
    } else {
        [bridge_ callCpp:[messageHelper_ onFailedToShow]];
    }
}

- (void)interstitialAdDidClick:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == interstitialAd, @"");
    [bridge_ callCpp:[messageHelper_ onClicked]];
}

- (void)interstitialAdDidClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == interstitialAd, @"");
    [bridge_ callCpp:[messageHelper_ onClosed]];
}

- (void)interstitialAdWillClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == interstitialAd, @"");
}

- (void)interstitialAdDidLoad:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == interstitialAd, @"");
    [bridge_ callCpp:[messageHelper_ onLoaded]];
}

- (void)interstitialAd:(FBInterstitialAd*)interstitialAd
      didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(ad_ == interstitialAd, @"");
    [bridge_ callCpp:[messageHelper_ onFailedToLoad]
             message:[error description]];
}

- (void)interstitialAdWillLogImpression:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == interstitialAd, @"");
}

@end
