//
//  EEALovinAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/applovin/EEAppLovin.h"

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

#import <AppLovinSDK/AppLovinSDK.h>

@interface EEAppLovin () <ALAdRewardDelegate, ALAdDisplayDelegate,
                          ALAdLoadDelegate>
@end

@implementation EEAppLovin

// clang-format off
NSString* const k__ALovinads_initALovinAds          = @"k__ALovinads_initALovinAds";
NSString* const k__ALovinads_isInterstitialReady    = @"k__ALovinads_isInterstitialReady";
NSString* const k__ALovinads_showInterstitial       = @"k__ALovinads_showInterstitial";
NSString* const k__ALovinads_isRewardVideoReady     = @"k__ALovinads_isRewardVideoReady";
NSString* const k__ALovinads_showRewardVideo        = @"k__ALovinads_showRewardVideo";
NSString* const k__AppLovin_cpp_callback            = @"__ALovinAds_callback";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:^(NSString* msg) {
        [self initialize];
        return @"";
    }
                        tag:k__ALovinads_initALovinAds];

    [bridge registerHandler:^(NSString* msg) {
        return [self isInterstitialReady] ? @"true" : @"false";
    }
                        tag:k__ALovinads_isInterstitialReady];

    [bridge registerHandler:^(NSString* msg) {
        return [self showInterstitial] ? @"true" : @"false";
    }
                        tag:k__ALovinads_showInterstitial];

    [bridge registerHandler:^(NSString* msg) {
        return [self isRewardVideoReady] ? @"true" : @"false";
    }
                        tag:k__ALovinads_isRewardVideoReady];

    [bridge registerHandler:^(NSString* msg) {
        return [self showRewardVideo] ? @"true" : @"false";
    }
                        tag:k__ALovinads_showRewardVideo];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__ALovinads_initALovinAds];
    [bridge deregisterHandler:k__ALovinads_isInterstitialReady];
    [bridge deregisterHandler:k__ALovinads_showInterstitial];
    [bridge deregisterHandler:k__ALovinads_isRewardVideoReady];
    [bridge deregisterHandler:k__ALovinads_showRewardVideo];
}

- (void)initialize {
    [ALSdk initializeSdk];

    [[ALIncentivizedInterstitialAd shared] setAdDisplayDelegate:self];
    [ALIncentivizedInterstitialAd preloadAndNotify:self];
}

- (BOOL)isInterstitialReady {
    return [ALInterstitialAd isReadyForDisplay];
}

- (BOOL)showInterstitial {
    if ([self isInterstitialReady]) {
        [ALInterstitialAd show];
        return YES;
    }
    return NO;
}

- (BOOL)isRewardVideoReady {
    return [ALIncentivizedInterstitialAd isReadyForDisplay];
}

- (BOOL)showRewardVideo {
    if ([self isRewardVideoReady]) {
        [ALIncentivizedInterstitialAd showAndNotify:self];
        return YES;
    }
    return NO;
}

- (void)adService:(ALAdService*)adService didLoadAd:(ALAd*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)adService:(ALAdService*)adService didFailToLoadAdWithError:(int)code {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasClickedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasDisplayedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasHiddenIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    // The user has closed the ad.  We must preload the next rewarded video.
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    [dict setValue:@2 forKey:@"code"];
    [dict setValue:@"videoReward" forKey:@"placement"];
    [[EEMessageBridge getInstance]
        callCpp:k__AppLovin_cpp_callback
            msg:[EEJsonUtils convertDictionaryToString:dict]];

    // FIXME: have to preload manually.
    [ALIncentivizedInterstitialAd preloadAndNotify:nil];
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
              didSucceedWithResponse:(NSDictionary*)response {
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
          didExceedQuotaWithResponse:(NSDictionary*)response {
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
             wasRejectedWithResponse:(NSDictionary*)response {
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
                    didFailWithError:(NSInteger)responseCode {
    NSLog(@"%s: code = %ld", __PRETTY_FUNCTION__, responseCode);
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    [dict setValue:@(responseCode) forKey:@"code"];
    [dict setValue:@"videoReward" forKey:@"placement"];
    [[EEMessageBridge getInstance]
        callCpp:k__AppLovin_cpp_callback
            msg:[EEJsonUtils convertDictionaryToString:dict]];
}
@end
