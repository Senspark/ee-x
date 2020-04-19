//
//  EEALovinAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/app_lovin/EEAppLovin.h"

#ifdef EE_X_USE_IRON_SOURCE_MEDIATION
#import <ISAppLovinAdapter/ALIncentivizedInterstitialAd.h>
#import <ISAppLovinAdapter/ALSdk.h>
#else // EE_X_USE_IRON_SOURCE_MEDIATION
#import <AppLovinSDK/ALAdRewardDelegate.h>
#import <AppLovinSDK/ALIncentivizedInterstitialAd.h>
#import <AppLovinSDK/ALInterstitialAd.h>
#import <AppLovinSDK/ALSdk.h>
#endif // EE_X_USE_IRON_SOURCE_MEDIATION

#import <ee/core/internal/EEDictionaryUtils.h>
#import <ee/core/internal/EEJsonUtils.h>
#import <ee/core/internal/EEMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

#define kPrefix @"AppLovin"

// clang-format off
static NSString* const k__initialize         = kPrefix "_initialize";
static NSString* const k__setTestAdsEnabled  = kPrefix "_setTestAdsEnabled";
static NSString* const k__setVerboseLogging  = kPrefix "_setVerboseLogging";
static NSString* const k__setMuted           = kPrefix "_setMuted";

static NSString* const k__hasInterstitialAd  = kPrefix "_hasInterstitialAd";
static NSString* const k__loadInterstitialAd = kPrefix "_loadInterstitialAd";
static NSString* const k__showInterstitialAd = kPrefix "_showInterstitialAd";

static NSString* const k__loadRewardedAd     = kPrefix "_loadRewardedAd";
static NSString* const k__hasRewardedAd      = kPrefix "_hasRewardedAd";
static NSString* const k__showRewardedAd     = kPrefix "_showRewardedAd";

static NSString* const k__onInterstitialAdLoaded       = kPrefix "_onInterstitialAdLoaded";
static NSString* const k__onInterstitialAdFailedToLoad = kPrefix "_onInterstitialAdFailedToLoad";
static NSString* const k__onInterstitialAdClicked      = kPrefix "_onInterstitialAdClicked";
static NSString* const k__onInterstitialAdClosed       = kPrefix "_onInterstitialAdClosed";

static NSString* const k__onRewardedAdLoaded       = kPrefix "_onRewardedAdLoaded";
static NSString* const k__onRewardedAdFailedToLoad = kPrefix "_onRewardedAdFailedToLoad";
static NSString* const k__onRewardedAdClicked      = kPrefix "_onRewardedAdClicked";
static NSString* const k__onRewardedAdClosed       = kPrefix "_onRewardedAdClosed";
// clang-format on

#undef kPrefix

@interface EEAppLovinInterstitialAdDelegate
    : NSObject <ALAdLoadDelegate, ALAdDisplayDelegate>
@end

@implementation EEAppLovinInterstitialAdDelegate {
    id<EEIMessageBridge> bridge_;
}

- (id)initWithBridge:(id<EEIMessageBridge>)bridge {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return nil;
    }
    bridge_ = bridge;
    return self;
}

- (void)adService:(ALAdService*)adService didLoadAd:(ALAd*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdLoaded];
}

- (void)adService:(ALAdService*)adService didFailToLoadAdWithError:(int)code {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdFailedToLoad
             message:[@(code) stringValue]];
}

- (void)ad:(ALAd*)ad wasClickedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdClicked];
}

- (void)ad:(ALAd*)ad wasDisplayedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasHiddenIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onInterstitialAdClosed];
}

@end

@interface EEAppLovinRewardedAdDelegate
    : NSObject <ALAdLoadDelegate, ALAdDisplayDelegate, ALAdRewardDelegate>
@end

@implementation EEAppLovinRewardedAdDelegate {
    id<EEIMessageBridge> bridge_;

@public
    BOOL rewarded_;
}

- (id)initWithBridge:(id<EEIMessageBridge>)bridge {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return nil;
    }
    bridge_ = bridge;
    return self;
}

- (void)adService:(ALAdService*)adService didLoadAd:(ALAd*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onRewardedAdLoaded];
}

- (void)adService:(ALAdService*)adService didFailToLoadAdWithError:(int)code {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onRewardedAdFailedToLoad message:[@(code) stringValue]];
}

- (void)ad:(ALAd*)ad wasClickedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onRewardedAdClicked];
}

- (void)ad:(ALAd*)ad wasDisplayedIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)ad:(ALAd*)ad wasHiddenIn:(UIView*)view {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:k__onRewardedAdClosed
             message:[EEUtils toString:rewarded_]];
}

- (void)rewardValidationRequestForAd:(ALAd*)ad
              didSucceedWithResponse:(NSDictionary*)response {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, response);
    rewarded_ = YES;
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

@end

@implementation EEAppLovin {
    id<EEIMessageBridge> bridge_;
    BOOL initialized_;
    ALSdk* sdk_;
    ALInterstitialAd* interstitialAd_;
    EEAppLovinInterstitialAdDelegate* interstitialAdDelegate_;
    ALIncentivizedInterstitialAd* rewardedAd_;
    EEAppLovinRewardedAdDelegate* rewardedAdDelegate_;
}

- (id)init {
    NSAssert([EEUtils isMainThread], @"");
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
    [super dealloc];
}

- (void)destroy {
    NSAssert([EEUtils isMainThread], @"");
    [self deregisterHandlers];
    if (!initialized_) {
        return;
    }
    [interstitialAd_ setAdLoadDelegate:nil];
    [interstitialAd_ setAdDisplayDelegate:nil];
    [interstitialAd_ release];
    interstitialAd_ = nil;
    [interstitialAdDelegate_ release];
    interstitialAdDelegate_ = nil;
    [rewardedAd_ setAdDisplayDelegate:nil];
    [rewardedAd_ release];
    rewardedAd_ = nil;
    [rewardedAdDelegate_ release];
    rewardedAdDelegate_ = nil;
    sdk_ = nil;
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

    [bridge_ registerHandler:k__loadInterstitialAd
                    callback:^(NSString* message) {
                        [self loadInterstitialAd];
                        return @"";
                    }];

    [bridge_ registerHandler:k__showInterstitialAd
                    callback:^(NSString* message) {
                        [self showInterstitialAd];
                        return @"";
                    }];

    [bridge_ registerHandler:k__hasRewardedAd
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self hasRewardedAd]];
                    }];

    [bridge_ registerHandler:k__loadRewardedAd
                    callback:^(NSString* message) {
                        [self loadRewardedAd];
                        return @"";
                    }];

    [bridge_ registerHandler:k__showRewardedAd
                    callback:^(NSString* message) {
                        [self showRewardedAd];
                        return @"";
                    }];
}

- (void)deregisterHandlers {
    [bridge_ deregisterHandler:k__initialize];
    [bridge_ deregisterHandler:k__setTestAdsEnabled];
    [bridge_ deregisterHandler:k__setVerboseLogging];
    [bridge_ deregisterHandler:k__setMuted];
    [bridge_ deregisterHandler:k__hasInterstitialAd];
    [bridge_ deregisterHandler:k__loadInterstitialAd];
    [bridge_ deregisterHandler:k__showInterstitialAd];
    [bridge_ deregisterHandler:k__hasRewardedAd];
    [bridge_ deregisterHandler:k__loadRewardedAd];
    [bridge_ deregisterHandler:k__showRewardedAd];
}

- (void)initialize:(NSString* _Nonnull)key {
    NSAssert([EEUtils isMainThread], @"");
    if (initialized_) {
        return;
    }
    sdk_ = [ALSdk sharedWithKey:key];
    [sdk_ initializeSdk];

    interstitialAd_ = [[ALInterstitialAd alloc] initWithSdk:sdk_];
    interstitialAdDelegate_ =
        [[EEAppLovinInterstitialAdDelegate alloc] initWithBridge:bridge_];
    [interstitialAd_ setAdLoadDelegate:interstitialAdDelegate_];
    [interstitialAd_ setAdDisplayDelegate:interstitialAdDelegate_];

    rewardedAd_ = [[ALIncentivizedInterstitialAd alloc] initWithSdk:sdk_];
    rewardedAdDelegate_ =
        [[EEAppLovinRewardedAdDelegate alloc] initWithBridge:bridge_];
    [rewardedAd_ setAdDisplayDelegate:rewardedAdDelegate_];

    initialized_ = YES;
}

- (void)setTestAdsEnabled:(BOOL)enabled {
    // Removed.
}

- (void)setVerboseLogging:(BOOL)enabled {
    NSAssert([EEUtils isMainThread], @"");
    [[sdk_ settings] setIsVerboseLogging:enabled];
}

- (void)setMuted:(BOOL)enabled {
    NSAssert([EEUtils isMainThread], @"");
    [[sdk_ settings] setMuted:enabled];
}

- (BOOL)hasInterstitialAd {
    NSAssert([EEUtils isMainThread], @"");
    // FIXME: use boolean variable.
    return [interstitialAd_ isReadyForDisplay];
}

- (void)loadInterstitialAd {
    NSAssert([EEUtils isMainThread], @"");
    [[sdk_ adService] loadNextAd:[ALAdSize interstitial]
                       andNotify:interstitialAdDelegate_];
}

- (void)showInterstitialAd {
    NSAssert([EEUtils isMainThread], @"");
    [interstitialAd_ show];
}

- (BOOL)hasRewardedAd {
    return [rewardedAd_ isReadyForDisplay];
}

- (void)loadRewardedAd {
    NSAssert([EEUtils isMainThread], @"");
    [rewardedAd_ preloadAndNotify:rewardedAdDelegate_];
}

- (void)showRewardedAd {
    NSAssert([EEUtils isMainThread], @"");
    rewardedAdDelegate_->rewarded_ = NO;
    [rewardedAd_ showAndNotify:rewardedAdDelegate_];
}

@end
