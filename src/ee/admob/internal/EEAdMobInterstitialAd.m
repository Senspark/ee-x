//
//  EEAdMobInterstitialAd.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <GoogleMobileAds/GoogleMobileAds.h>

#import "ee/admob/internal/EEAdMobInterstitialAd.h"
#import "ee/ads/internal/EEInterstitialAdHelper.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"

@interface EEAdMobInterstitialAd () <GADInterstitialDelegate> {
    NSString* adId_;
    GADInterstitial* interstitialAd_;
    NSArray<NSString*>* testDevices_;
    EEInterstitialAdHelper* helper_;
}

@end

@implementation EEAdMobInterstitialAd

- (id _Nonnull)initWithAdId:(NSString* _Nonnull)adId
                testDevices:(NSArray<NSString*>* _Nullable)testDevices {
    self = [super init];
    if (self == nil) {
        return self;
    }
    adId_ = [adId copy];
    interstitialAd_ = nil;
    testDevices_ = [testDevices retain];
    helper_ =
        [[EEInterstitialAdHelper alloc] initWithPrefix:@"AdMobInterstitialAd"
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
    [testDevices_ release];
    testDevices_ = nil;
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

- (void)registerHandlers {
    [helper_ registerHandlers:self];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__createInternalAd]
                   callback:^(NSString* message) {
                       return [EEUtils toString:[self createInternalAd]];
                   }];

    [bridge registerHandler:[self k__destroyInternalAd]
                   callback:^(NSString* message) {
                       return [EEUtils toString:[self destroyInternalAd]];
                   }];
}

- (void)deregisterHandlers {
    [helper_ deregisterHandlers];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__createInternalAd]];
    [bridge deregisterHandler:[self k__destroyInternalAd]];
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
    [request setTestDevices:testDevices_];
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

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onLoaded]];
}

- (void)interstitial:(GADInterstitial*)ad
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(interstitialAd_ == ad, @"");

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)interstitialWillPresentScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");
}

- (void)interstitialDidFailToPresentScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onFailedToShow]];
}

- (void)interstitialWillDismissScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");
}

- (void)interstitialDidDismissScreen:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onClosed]];
}
- (void)interstitialWillLeaveApplication:(GADInterstitial*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(interstitialAd_ == ad, @"");
}

@end
