//
//  EEAdMobInterstitialAd.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/admob/internal/EEAdMobInterstitialAd.h"

#import <GoogleMobileAds/GoogleMobileAds.h>

#import <ee/ads/EEInterstitialAdHelper.h>
#import <ee/core/EEIMessageBridge.h>
#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEUtils.h>

@interface EEAdMobInterstitialAd () <GADInterstitialDelegate> {
    id<EEIMessageBridge> bridge_;
    NSString* adId_;
    GADInterstitial* interstitialAd_;
    EEInterstitialAdHelper* helper_;
}

@end

@implementation EEAdMobInterstitialAd

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    adId_ = [adId copy];
    interstitialAd_ = nil;
    helper_ =
        [[EEInterstitialAdHelper alloc] initWithBridge:bridge_
                                                prefix:@"AdMobInterstitialAd"
                                                  adId:adId_];
    [self registerHandlers];
    return self;
}

- (void)destroy {
    [self deregisterHandlers];
}

- (void)dealloc {
    [helper_ release];
    helper_ = nil;
    [adId_ release];
    adId_ = nil;
    [super dealloc];
}

- (NSString*)k__createInternalAd {
    return [@"AdMobInterstitialAd_createInternalAd_"
        stringByAppendingString:adId_];
}

- (NSString*)k__destroyInternalAd {
    return [@"AdMobInterstitialAd_destroyInternalAd_"
        stringByAppendingString:adId_];
}

- (NSString*)k__onLoaded {
    return [@"AdMobInterstitialAd_onLoaded_" stringByAppendingString:adId_];
}

- (NSString*)k__onFailedToLoad {
    return
        [@"AdMobInterstitialAd_onFailedToLoad_" stringByAppendingString:adId_];
}

- (NSString*)k__onFailedToShow {
    return
        [@"AdMobInterstitialAd_onFailedToShow_" stringByAppendingString:adId_];
}

- (NSString*)k__onClosed {
    return [@"AdMobInterstitialAd_onClosed_" stringByAppendingString:adId_];
}

- (NSString* _Nonnull)k__onClicked {
    return [@"AdMobInterstitialAd_onClicked_" stringByAppendingString:adId_];
}

- (void)registerHandlers {
    [helper_ registerHandlers:self];
    [bridge_ registerHandler:[self k__createInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self createInternalAd]];
                    }];

    [bridge_ registerHandler:[self k__destroyInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self destroyInternalAd]];
                    }];
}

- (void)deregisterHandlers {
    [helper_ deregisterHandlers];
    [bridge_ deregisterHandler:[self k__createInternalAd]];
    [bridge_ deregisterHandler:[self k__destroyInternalAd]];
}

- (BOOL)createInternalAd {
    if (interstitialAd_ != nil) {
        return NO;
    }
    GADInterstitial* interstitialAd =
        [[[GADInterstitial alloc] initWithAdUnitID:adId_] autorelease];
    [interstitialAd setDelegate:self];
    interstitialAd_ = [interstitialAd retain];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (interstitialAd_ == nil) {
        return NO;
    }
    [interstitialAd_ setDelegate:nil];
    [interstitialAd_ release];
    interstitialAd_ = nil;
    return YES;
}

- (BOOL)isLoaded {
    if (interstitialAd_ == nil) {
        return NO;
    }
    return [interstitialAd_ isReady];
}

- (void)load {
    if (interstitialAd_ == nil) {
        return;
    }
    GADRequest* request = [GADRequest request];
    [interstitialAd_ loadRequest:request];
}

- (BOOL)show {
    if (interstitialAd_ == nil) {
        return NO;
    }
    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [interstitialAd_ presentFromRootViewController:rootView];
    return YES;
}

- (void)interstitialDidReceiveAd:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");
    [bridge_ callCpp:[self k__onLoaded]];
}

- (void)interstitial:(GADInterstitial*)ad
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(interstitialAd_ == ad, @"");
    [bridge_ callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)interstitialWillPresentScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");
}

- (void)interstitialDidFailToPresentScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");
    [bridge_ callCpp:[self k__onFailedToShow]];
}

- (void)interstitialWillDismissScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");
}

- (void)interstitialDidDismissScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");
    [bridge_ callCpp:[self k__onClosed]];
}

/// Tells the delegate that a user click will open another app
/// (such as the App Store), backgrounding the current app.
- (void)interstitialWillLeaveApplication:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");
    [bridge_ callCpp:[self k__onClicked]];
}

@end
