//
//  EEAdMobInterstitialAd.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/admob/private/EEAdMobInterstitialAd.h"

#import <GoogleMobileAds/GoogleMobileAds.h>

#import <ee/ads/internal/EEInterstitialAdHelper.h>
#import <ee/ads/internal/EEMessageHelper.h>
#import <ee/core/internal/EEIMessageBridge.h>
#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEUtils.h>

@interface EEAdMobInterstitialAd () <GADInterstitialDelegate>
@end

@implementation EEAdMobInterstitialAd {
    id<EEIMessageBridge> bridge_;
    NSString* adId_;
    EEMessageHelper* messageHelper_;
    EEInterstitialAdHelper* helper_;
    GADInterstitial* ad_;
}

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    adId_ = [adId copy];
    messageHelper_ =
        [[EEMessageHelper alloc] initWithPrefix:@"AdMobInterstitialAd"
                                           adId:adId];
    helper_ = [[EEInterstitialAdHelper alloc] initWithBridge:bridge_
                                                          ad:self
                                                      helper:messageHelper_];
    [self createInternalAd];
    [self registerHandlers];
    return self;
}

- (void)destroy {
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
    if (ad_ != nil) {
        return NO;
    }
    ad_ = [[GADInterstitial alloc] initWithAdUnitID:adId_];
    [ad_ setDelegate:self];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (ad_ == nil) {
        return NO;
    }
    [ad_ setDelegate:nil];
    [ad_ release];
    ad_ = nil;
    return YES;
}

- (BOOL)isLoaded {
    NSAssert(ad_ != nil, @"");
    return [ad_ isReady];
}

- (void)load {
    NSAssert(ad_ != nil, @"");
    GADRequest* request = [GADRequest request];
    [ad_ loadRequest:request];
}

- (void)show {
    NSAssert(ad_ != nil, @"");
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [ad_ presentFromRootViewController:rootView];
}

- (void)interstitialDidReceiveAd:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == ad, @"");
    [bridge_ callCpp:[messageHelper_ onLoaded]];
}

- (void)interstitial:(GADInterstitial*)ad
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(ad_ == ad, @"");
    [bridge_ callCpp:[messageHelper_ onFailedToLoad]
             message:[error description]];
}

- (void)interstitialWillPresentScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == ad, @"");
}

- (void)interstitialDidFailToPresentScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == ad, @"");
    [bridge_ callCpp:[messageHelper_ onFailedToShow]];
}

- (void)interstitialWillDismissScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == ad, @"");
}

- (void)interstitialDidDismissScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == ad, @"");
    [bridge_ callCpp:[messageHelper_ onClosed]];
}

/// Tells the delegate that a user click will open another app
/// (such as the App Store), backgrounding the current app.
- (void)interstitialWillLeaveApplication:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == ad, @"");
    [bridge_ callCpp:[messageHelper_ onClicked]];
}

@end
