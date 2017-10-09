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
static NSString* const k__initialize            = @"AppLovin_initialize";
static NSString* const k__isInterstitialAdReady = @"AppLovin_isInterstitialAdReady";
static NSString* const k__showInterstitialAd    = @"AppLovin_showInterstitialAd";
static NSString* const k__showRewardedVideo     = @"AppLovin_showRewardedVideo";
static NSString* const k__cppCallback           = @"AppLovin_cppCallback";
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
    [[ALIncentivizedInterstitialAd shared] setAdDisplayDelegate:nil];
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__initialize
                   callback:^(NSString* message) {
                       [self initialize];
                       return @"";
                   }];

    [bridge registerHandler:k__isInterstitialAdReady
                   callback:^(NSString* message) {
                       return [self isInterstitialReady] ? @"true" : @"false";
                   }];

    [bridge registerHandler:k__showInterstitialAd
                   callback:^(NSString* message) {
                       return [self showInterstitial] ? @"true" : @"false";
                   }];

    [bridge registerHandler:k__showRewardedVideo
                   callback:^(NSString* message) {
                       return [self showRewardVideo] ? @"true" : @"false";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__initialize];
    [bridge deregisterHandler:k__isInterstitialAdReady];
    [bridge deregisterHandler:k__showInterstitialAd];
    [bridge deregisterHandler:k__showRewardedVideo];
}

- (void)initialize {
    [ALSdk initializeSdk];
    [[ALIncentivizedInterstitialAd shared] setAdDisplayDelegate:self];
    [self loadRewardedVideo];
}

- (BOOL)isInterstitialReady {
    return [ALInterstitialAd isReadyForDisplay];
}

- (BOOL)showInterstitial {
    if (![self isInterstitialReady]) {
        return NO;
    }
    [ALInterstitialAd show];
    return YES;
}

- (void)loadRewardedVideo {
    [ALIncentivizedInterstitialAd preloadAndNotify:self];
}

- (BOOL)isRewardVideoReady {
    return [ALIncentivizedInterstitialAd isReadyForDisplay];
}

- (BOOL)showRewardVideo {
    if (![self isRewardVideoReady]) {
        [self loadRewardedVideo];
        return NO;
    }
    [ALIncentivizedInterstitialAd showAndNotify:self];
    return YES;
}

- (void)sendResult:(BOOL)result {
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    [dict setValue:@(result) forKey:@"result"];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:k__cppCallback
            message:[EEJsonUtils convertDictionaryToString:dict]];
}

- (void)adService:(ALAdService*)adService didLoadAd:(ALAd*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)adService:(ALAdService*)adService didFailToLoadAdWithError:(int)code {
    NSLog(@"%s: code = %d", __PRETTY_FUNCTION__, code);
}

- (void)ad:(ALAd*)ad wasClickedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasDisplayedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasHiddenIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self loadRewardedVideo];
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
              didSucceedWithResponse:(NSDictionary*)response {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self sendResult:YES];
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
          didExceedQuotaWithResponse:(NSDictionary*)response {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self sendResult:NO];
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
             wasRejectedWithResponse:(NSDictionary*)response {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self sendResult:NO];
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
                    didFailWithError:(NSInteger)responseCode {
    NSLog(@"%s: code = %ld", __PRETTY_FUNCTION__, responseCode);
    [self sendResult:NO];
}

- (void)userDeclinedToViewAd:(ALAd*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self sendResult:NO];
}

@end
