//
//  EEALovinAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/alovin/EEALovinAds.h"

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

#import <AppLovinSDK/AppLovinSDK.h>

@interface EEALovinAds () <ALAdRewardDelegate, ALAdDisplayDelegate, ALAdLoadDelegate>{
}

@end

@implementation EEALovinAds

NSString* const k__ALovinads_initALovinAds = @"k__ALovinads_initALovinAds";
NSString* const k__ALovinads_isInterstitialReady = @"k__ALovinads_isInterstitialReady";
NSString* const k__ALovinads_showInterstitial= @"k__ALovinads_showInterstitial";
NSString* const k__ALovinads_isRewardVideoReady = @"k__ALovinads_isRewardVideoReady";
NSString* const k__ALovinads_showRewardVideo = @"k__ALovinads_showRewardVideo";

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
        
        [self initALovinAds];

        return [EEDictionaryUtils emptyResult];
    } tag:k__ALovinads_initALovinAds];

    [bridge registerHandler:^(NSString* msg) {
        return ([self isInterstitialReady]) ? @"true" : @"false";
    } tag:k__ALovinads_isInterstitialReady];

    [bridge registerHandler:^(NSString* msg) {
        [self showInterstitial];

        return [EEDictionaryUtils emptyResult];
    } tag:k__ALovinads_showInterstitial];
    
    [bridge registerHandler:^(NSString* msg) {
        return ([self isRewardVideoReady]) ? @"true" : @"false";
    } tag:k__ALovinads_isRewardVideoReady];
    
    [bridge registerHandler:^(NSString* msg) {
        [self showRewardVideo];
        
        return [EEDictionaryUtils emptyResult];
    } tag:k__ALovinads_showRewardVideo];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__ALovinads_initALovinAds];
    [bridge deregisterHandler:k__ALovinads_isInterstitialReady];
    [bridge deregisterHandler:k__ALovinads_showInterstitial];
    [bridge deregisterHandler:k__ALovinads_isRewardVideoReady];
    [bridge deregisterHandler:k__ALovinads_showRewardVideo];
}
#pragma mark ===================CODE HERE
- (void)initALovinAds{
    
    [ALSdk initializeSdk];
    
    [ALIncentivizedInterstitialAd shared].adDisplayDelegate = self;
    
    [ALIncentivizedInterstitialAd preloadAndNotify: self];
    
    _rootController =
        [[[UIApplication sharedApplication] keyWindow] rootViewController];
}

- (BOOL) isInterstitialReady
{
    return [ALInterstitialAd isReadyForDisplay];
}

- (void) showInterstitial
{
    if ([ALInterstitialAd isReadyForDisplay]) {
        [ALInterstitialAd show];
    }
}

- (BOOL) isRewardVideoReady
{
    return [ALIncentivizedInterstitialAd isReadyForDisplay];
}

- (void) showRewardVideo
{
    [ALIncentivizedInterstitialAd showAndNotify: self];
}

#pragma mark ===================ALAdLoadDelegate <NSObject>
- (void)adService:(ALAdService *)adService didLoadAd:(ALAd *)ad
{
    NSLog(@"EEALovin ADS didLoadAd");
}

- (void)adService:(ALAdService *)adService didFailToLoadAdWithError:(int)code
{
    NSLog(@"EEALovin ADS didFailToLoadAdWithError");
}

#pragma mark ===================ALAdDisplayDelegate
- (void)ad:(ALAd *)ad wasClickedIn:(UIView *)view{}
- (void)ad:(ALAd *)ad wasDisplayedIn:(UIView *)view{}
- (void)ad:(ALAd *)ad wasHiddenIn:(UIView *)view {
    // The user has closed the ad.  We must preload the next rewarded video.
    [ALIncentivizedInterstitialAd preloadAndNotify:nil];
}
#pragma mark ===================ALAdRewardDelegate

- (void)rewardValidationRequestForAd:(ALAd *)ad didSucceedWithResponse:(NSDictionary *)response
{
//    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
//    [dict setValue:[NSNumber numberWithInteger:state] forKey:@"code"];
//    [dict setValue:placementId forKey:@"placement"];
    
    NSLog(@"EEALovin ADS   finish dict %@",
          [EEJsonUtils convertDictionaryToString:response]);
    [[EEMessageBridge getInstance]
     callCpp:@"__ALovinAds_callback"
     msg:[EEJsonUtils convertDictionaryToString:response]];
}

- (void)rewardValidationRequestForAd:(ALAd *)ad didExceedQuotaWithResponse:(NSDictionary *)response
{
    
}

- (void)rewardValidationRequestForAd:(ALAd *)ad wasRejectedWithResponse:(NSDictionary *)response
{
    
}

- (void)rewardValidationRequestForAd:(ALAd *)ad didFailWithError:(NSInteger)responseCode
{
    NSLog(@"EEALovin ADS   error %d", responseCode);
    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
    [dict setValue:responseCode forKey:@"code"];
    [dict setValue:@"videoReward" forKey:@"placement"];
    
    NSLog(@"EEALovin ADS   error dict %@",
          [EEJsonUtils convertDictionaryToString:dict]);
    [[EEMessageBridge getInstance]
     callCpp:@"__ALovinAds_callback"
     msg:[EEJsonUtils convertDictionaryToString:dict]];
}
@end
