//
//  EEFacebookAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/facebook/EEFacebookAds.h"

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

#import "FBAudienceNetwork/FBAudienceNetwork.h"


#define EVENT_FBADS_INTERSTITIAL_ADS_LOADED "on_fbads_interstitial_ads_loaded"
#define EVENT_FBADS_INTERSTITIAL_ADS_FAILED "on_fbads_interstitial_ads_failed"
#define EVENT_FBADS_INTERSTITIAL_ADS_CLOSED "on_fbads_interstitial_ads_closed"

#undef CLS_LOG
#ifdef __OBJC__
#ifndef NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSNSLog(__FORMAT__, ##__VA_ARGS__)
#else // NDEBUG
#define CLS_LOG(__FORMAT__, ...) CLSLog(__FORMAT__, ##__VA_ARGS__)
#endif // NDEBUG
#endif // __OBJC__

@interface EEFacebookAds () <FBInterstitialAdDelegate, FBNativeAdDelegate>
{
    BOOL _canShowInterstitialAd;
}
@property (nonatomic, strong) FBInterstitialAd* interstitialAd;
@property (strong, nonatomic) FBNativeAd* nativeAd;
@end

@implementation EEFacebookAds

NSString* const k__facebookads_initFBAds = @"__facebookads_initFBAds";

NSString* const k__facebookads_cacheRewardedAd =
    @"__facebookads_cacheRewardedAd";
NSString* const k__facebookads_cacheInterstitialAd =
    @"__facebookads_cacheInterstitialAd";

NSString* const k__facebookads_showBannerAd = @"__facebookads_showBannerAd";
NSString* const k__facebookads_hideBannerAd = @"__facebookads_hideBannerAd";

NSString* const k__facebookads_hasInterstitialAd =
    @"__facebookads_hasInterstitialAd";
NSString* const k__facebookads_hasRewardedAd = @"__facebookads_hasRewardedAd";

NSString* const k__facebookads_showInterstitialAd =
    @"__facebookads_showInterstitialAd";
NSString* const k__facebookads_showRewardedAd = @"__facebookads_showRewardedAd";

NSString* const k__facebookads_showNativeExpressAd =
    @"__facebookads_showNativeExpressAd";

NSString* const k__facebookads_showNativeExpressAdWithDeltaPosition =
    @"__facebookads_showNativeExpressAdWithDeltaPosition";

NSString* const k__facebookads_hideNativeExpressAd =
    @"__facebookads_hideNativeExpressAd";

NSString* const k__facebookads_showNativeAdvancedAd =
    @"__facebookads_showNativeAdvancedAd";
NSString* const k__facebookads_hideNativeAdvancedAd =
    @"__facebookads_hideNativeAdvancedAd";

NSString* const k__facebookads_onAdsCallback = @"__facebookads_onAdsCallback";

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
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* InterstitialID = dict[@"InterstitialID"];
        NSString* NativeID = dict[@"NativeID"];
        NSString* BannerID = dict[@"BannerID"];

        [self initFBAds:InterstitialID nativeID:NativeID bannerID:BannerID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_initFBAds];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* adsID = dict[@"adsID"];

        [self cacheRewardedAd:adsID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_cacheRewardedAd];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* adsID = dict[@"adsID"];

        [self cacheInterstitialAd:adsID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_cacheInterstitialAd];

    [bridge registerHandler:^(NSString* msg) {

        [self showInterstitialAd];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_showInterstitialAd];

    [bridge registerHandler:^(NSString* msg) {

        [self showRewardedAd];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_showRewardedAd];

    [bridge registerHandler:^(NSString* msg) {

        [self hideBannerAd];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_hideBannerAd];

    [bridge registerHandler:^(NSString* msg) {

        [self hideNativeExpressAd];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_hideNativeExpressAd];

    [bridge registerHandler:^(NSString* msg) {

        [self hideNativeAdvancedAd];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_hideNativeAdvancedAd];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* adsID = dict[@"adsID"];
        int width = dict[@"width"];
        int height = dict[@"height"];
        int pos = dict[@"pos"];

        [self showNativeExpressAd:adsID width:width height:height pos:pos];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_showNativeExpressAd];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* adsID = dict[@"adsID"];
        int width = dict[@"width"];
        int height = dict[@"height"];
        int deltaX = dict[@"deltaX"];
        int deltaY = dict[@"deltaY"];

        [self showNativeExpressAdWithDeltaPosition:adsID
                                             width:width
                                            height:height
                                            deltaX:deltaX
                                            deltaY:deltaY];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_showNativeExpressAdWithDeltaPosition];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* adsID = dict[@"adsID"];
        int width = dict[@"width"];
        int height = dict[@"height"];
        int x = dict[@"x"];
        int y = dict[@"y"];

        [self showNativeAdvancedAd:adsID width:width height:height x:x y:y];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_showNativeAdvancedAd];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__facebookads_initFBAds];
}
#pragma mark ===================CODE HERE
- (void)initFBAds:(NSString*)InterstitialID
         nativeID:(NSString*)NativeID
         bannerID:(NSString*)BannerID {
    
    _canShowInterstitialAd = NO;
    
    // test device
    [FBAdSettings addTestDevice:[FBAdSettings testDeviceHash]];

    // inter ad
    self.interstitialAd =
        [[FBInterstitialAd alloc] initWithPlacementID:InterstitialID];
    self.interstitialAd.delegate = self;
    [self.interstitialAd loadAd];

    // native ad
    FBNativeAd* nativeAd = [[FBNativeAd alloc] initWithPlacementID:NativeID];
    nativeAd.delegate = self;
    nativeAd.mediaCachePolicy = FBNativeAdsCachePolicyAll;

    // banner ad
}

- (void)cacheRewardedAd:(NSString*)adsID {
}

- (void)cacheInterstitialAd:(NSString*)adsID {
}

- (void)showBannerAd:(NSString*)adsID pos:(int)pos {
}
- (void)hideBannerAd {
}

- (bool)hasInterstitialAd {
    return true;
}
- (bool)hasRewardedAd {
    return true;
}

- (void)showInterstitialAd {
    NSLog(@"FBAds ======== showInterstitialAd");
    if (_canShowInterstitialAd)
    {
        UIViewController* root =
        [[[UIApplication sharedApplication] keyWindow] rootViewController];
        
        [self.interstitialAd showAdFromRootViewController:root];
        
        _canShowInterstitialAd = NO;
    }
}
- (void)showRewardedAd {
}

- (void)showNativeExpressAd:(NSString*)adsID
                      width:(int)width
                     height:(int)height
                        pos:(int)pos {
}
- (void)showNativeExpressAdWithDeltaPosition:(NSString*)adsId
                                       width:(int)width
                                      height:(int)height
                                      deltaX:(int)deltaX
                                      deltaY:(int)deltaY {
}
- (void)hideNativeExpressAd {
}

- (void)showNativeAdvancedAd:(NSString*)adsID
                       width:(int)width
                      height:(int)height
                           x:(int)x
                           y:(int)y {
}

- (void)hideNativeAdvancedAd {
}

- (void)onAdsCallback:(int)code msg:(NSString*)msg {
}

#pragma mark ============FBInterstitialAdDelegate
- (void)interstitialAdDidClick:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdDidClick");
}
- (void)interstitialAdDidClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdDidClose");
    
    [self.interstitialAd loadAd];
    
    
}
- (void)interstitialAdWillClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdWillClose");
}
- (void)interstitialAdDidLoad:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdDidLoad");

    _canShowInterstitialAd = YES;
    
}
- (void)interstitialAd:(FBInterstitialAd*)interstitialAd
      didFailWithError:(NSError*)error {
    NSLog(@"FBAds ======== interstitialAd didFailWithError %s",
          error.description);
}
- (void)interstitialAdWillLogImpression:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdWillLogImpression");
}

#pragma mark ============FBNativeAdDelegate
@end
