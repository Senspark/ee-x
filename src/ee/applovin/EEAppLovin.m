//
//  EEALovinAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/applovin/EEAppLovin.h"

#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"

#import <AppLovinSDK/AppLovinSDK.h>

// clang-format off
static NSString* const k__initialize               = @"AppLovin_initialize";
static NSString* const k__setTestAdsEnabled        = @"AppLovin_setTestAdsEnabled";
static NSString* const k__setVerboseLogging        = @"AppLovin_setVerboseLogging";
static NSString* const k__setMuted                 = @"AppLovin_setMuted";
static NSString* const k__hasInterstitialAd        = @"AppLovin_hasInterstitialAd";
static NSString* const k__showInterstitialAd       = @"AppLovin_showInterstitialAd";
static NSString* const k__loadRewardedVideo        = @"AppLovin_loadRewardedVideo";
static NSString* const k__hasRewardedVideo         = @"AppLovin_hasRewardedVideo";
static NSString* const k__showRewardedVideo        = @"AppLovin_showRewardedVideo";
static NSString* const k__onInterstitialAdHidden   = @"AppLovin_onInterstitialAdHidden";
static NSString* const k__onRewardedVideoFailed    = @"AppLovin_onRewardedVideoFailed";
static NSString* const k__onRewardedVideoDisplayed = @"AppLovin_onRewardedVideoDisplayed";
static NSString* const k__onRewardedVideoHidden    = @"AppLovin_onRewardedVideoHidden";
static NSString* const k__onUserRewardVerified     = @"AppLovin_onUserRewardVerified";
// clang-format on

@interface EEAppLovinIncentivizedInterstitialAdDisplayDelegate
    : NSObject <ALAdDisplayDelegate> {
    EEMessageBridge* bridge_;
}

@end

@interface EEAppLovinInterstitialAdDisplayDelegate
    : NSObject <ALAdDisplayDelegate> {
    EEMessageBridge* bridge_;
}

@end

@implementation EEAppLovinIncentivizedInterstitialAdDisplayDelegate

- (id)initWithBridge:(EEMessageBridge* _Nonnull)bridge {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    bridge_ = bridge;
    return self;
}

- (void)ad:(ALAd*)ad wasClickedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasDisplayedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasHiddenIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdHidden];
}

@end

@implementation EEAppLovinInterstitialAdDisplayDelegate

- (id)initWithBridge:(EEMessageBridge* _Nonnull)bridge {
    self = [super init];
    if (self == nil) {
        return nil;
    }
    bridge_ = bridge;
    return self;
}

- (void)ad:(ALAd*)ad wasClickedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasDisplayedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onRewardedVideoDisplayed];
}

- (void)ad:(ALAd*)ad wasHiddenIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onRewardedVideoHidden];
}

@end

@interface EEAppLovin () <ALAdRewardDelegate, ALAdLoadDelegate> {
    BOOL initialized_;
    EEMessageBridge* bridge_;
    ALSdk* sdk_;
    ALInterstitialAd* interstitialAd_;
    EEAppLovinInterstitialAdDisplayDelegate* interstitialAdDisplayDelegate_;
    ALIncentivizedInterstitialAd* incentivizedInterstitialAd_;
    EEAppLovinIncentivizedInterstitialAdDisplayDelegate*
        incentivizedInterstitialAdDisplayDelegate_;
}

@end

@implementation EEAppLovin

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    initialized_ = NO;
    bridge_ = [EEMessageBridge getInstance];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [self destroy];
    [super dealloc];
}

- (void)registerHandlers {
    [bridge_ registerHandler:k__initialize
                    callback:^(NSString* message) {
                        NSString* key = message;
                        [self initialize:key];
                        return @"";
                    }];

    [bridge_ registerHandler:k__setTestAdsEnabled
                    callback:^(NSString* message) {
                        [self setTestAdsEnabled:[EEUtils toBool:message]];
                        return @"";
                    }];

    [bridge_ registerHandler:k__setVerboseLogging
                    callback:^(NSString* message) {
                        [self setVerboseLogging:[EEUtils toBool:message]];
                        return @"";
                    }];

    [bridge_ registerHandler:k__setMuted
                    callback:^(NSString* message) {
                        [self setMuted:[EEUtils toBool:message]];
                        return @"";
                    }];

    [bridge_ registerHandler:k__hasInterstitialAd
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self hasInterstitialAd]];
                    }];

    [bridge_ registerHandler:k__showInterstitialAd
                    callback:^(NSString* message) {
                        [self showInterstitialAd];
                        return @"";
                    }];

    [bridge_ registerHandler:k__loadRewardedVideo
                    callback:^(NSString* message) {
                        [self loadRewardedVideo];
                        return @"";
                    }];

    [bridge_ registerHandler:k__hasRewardedVideo
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self hasRewardedVideo]];
                    }];

    [bridge_ registerHandler:k__showRewardedVideo
                    callback:^(NSString* message) {
                        [self showRewardedVideo];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__setTestAdsEnabled];
    [bridge_ deregisterHandler:k__setVerboseLogging];
    [bridge_ deregisterHandler:k__setMuted];
    [bridge_ deregisterHandler:k__hasInterstitialAd];
    [bridge_ deregisterHandler:k__showInterstitialAd];
    [bridge_ deregisterHandler:k__loadRewardedVideo];
    [bridge_ deregisterHandler:k__hasRewardedVideo];
    [bridge_ deregisterHandler:k__showRewardedVideo];
}

- (void)initialize:(NSString* _Nonnull)key {
    if (initialized_) {
        return;
    }
    sdk_ = [ALSdk sharedWithKey:key];
    [sdk_ initializeSdk];

    incentivizedInterstitialAd_ =
        [[ALIncentivizedInterstitialAd alloc] initWithSdk:sdk_];

    incentivizedInterstitialAdDisplayDelegate_ =
        [[EEAppLovinIncentivizedInterstitialAdDisplayDelegate alloc]
            initWithBridge:bridge_];
    [incentivizedInterstitialAd_
        setAdDisplayDelegate:incentivizedInterstitialAdDisplayDelegate_];

    interstitialAd_ = [[ALInterstitialAd alloc] initWithSdk:sdk_];
    interstitialAdDisplayDelegate_ = [
        [EEAppLovinInterstitialAdDisplayDelegate alloc] initWithBridge:bridge_];
    [interstitialAd_ setAdDisplayDelegate:interstitialAdDisplayDelegate_];

    initialized_ = YES;
}

- (void)destroy {
    if (!initialized_) {
        return;
    }
    [interstitialAd_ setAdDisplayDelegate:nil];
    [interstitialAd_ release];
    interstitialAd_ = nil;
    [incentivizedInterstitialAd_ setAdDisplayDelegate:nil];
    [incentivizedInterstitialAd_ release];
    incentivizedInterstitialAd_ = nil;
    [incentivizedInterstitialAdDisplayDelegate_ release];
    incentivizedInterstitialAdDisplayDelegate_ = nil;
    sdk_ = nil;
}

- (void)setTestAdsEnabled:(BOOL)enabled {
    [[sdk_ settings] setIsTestAdsEnabled:enabled];
}

- (void)setVerboseLogging:(BOOL)enabled {
    [[sdk_ settings] setIsVerboseLogging:enabled];
}

- (void)setMuted:(BOOL)enabled {
    [[sdk_ settings] setMuted:enabled];
}

- (BOOL)hasInterstitialAd {
    return [interstitialAd_ isReadyForDisplay];
}

- (void)showInterstitialAd {
    [interstitialAd_ show];
}

- (void)loadRewardedVideo {
    [incentivizedInterstitialAd_ preloadAndNotify:self];
}

- (BOOL)hasRewardedVideo {
    return [incentivizedInterstitialAd_ isReadyForDisplay];
}

- (void)showRewardedVideo {
    [incentivizedInterstitialAd_ showAndNotify:self];
}

- (void)adService:(ALAdService*)adService didLoadAd:(ALAd*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)adService:(ALAdService*)adService didFailToLoadAdWithError:(int)code {
    NSLog(@"%s: code = %d", __PRETTY_FUNCTION__, code);
    [bridge_ callCpp:k__onRewardedVideoFailed
             message:[NSString stringWithFormat:@"%d", code]];
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
              didSucceedWithResponse:(NSDictionary*)response {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, response);
    [bridge_ callCpp:k__onUserRewardVerified];
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
          didExceedQuotaWithResponse:(NSDictionary*)response {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, response);
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
             wasRejectedWithResponse:(NSDictionary*)response {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, response);
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
                    didFailWithError:(NSInteger)responseCode {
    NSLog(@"%s: code = %ld", __PRETTY_FUNCTION__, responseCode);
}

- (void)userDeclinedToViewAd:(ALAd*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
