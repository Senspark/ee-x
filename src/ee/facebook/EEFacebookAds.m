//
//  EEFacebookAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import "ee/facebook/EEFacebookAds.h"
#import "ee/facebook/EEFacebookAdsUIView.h"
#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"

#import <FBAudienceNetwork/FBAudienceNetwork.h>

@interface EEFacebookAds () <FBInterstitialAdDelegate, FBNativeAdDelegate,
                             FBMediaViewDelegate> {
    BOOL _canShowInterstitialAd;

    NSString* _interstitialID;

    NSMutableDictionary* _dictFBNativeAd;
    NSMutableDictionary* _dictFBNativeAdView;
    NSMutableDictionary* _dictFBNativeAdReady;
}

@property (nonatomic, strong) FBInterstitialAd* interstitialAd;

@end

@implementation EEFacebookAds

// clang-format off
NSString* const k__facebookads_initFBAdsInterstitial =  @"__facebookads_initFBAdsInterstitial";
NSString* const k__facebookads_initFBAdsNativeAds =     @"__facebookads_initFBAdsNativeAds";
NSString* const k__facebookads_initFBAdsBanner =        @"__facebookads_initFBAdsBanner";

NSString* const k__facebookads_cacheRewardedAd =        @"__facebookads_cacheRewardedAd";
NSString* const k__facebookads_cacheInterstitialAd =    @"__facebookads_cacheInterstitialAd";

NSString* const k__facebookads_showBannerAd =           @"__facebookads_showBannerAd";
NSString* const k__facebookads_hideBannerAd =           @"__facebookads_hideBannerAd";

NSString* const k__facebookads_hasNativeAd =            @"__facebookads_hasNativeAd";
NSString* const k__facebookads_hasInterstitialAd =      @"__facebookads_hasInterstitialAd";
NSString* const k__facebookads_hasRewardedAd =          @"__facebookads_hasRewardedAd";

NSString* const k__facebookads_showInterstitialAd =     @"__facebookads_showInterstitialAd";
NSString* const k__facebookads_showRewardedAd =         @"__facebookads_showRewardedAd";

NSString* const k__facebookads_hideNativeAd =           @"__facebookads_hideNativeAd";
NSString* const k__facebookads_showNativeAd =           @"__facebookads_showNativeAd";

NSString* const k__facebookads_callback =               @"__facebookads_callback";
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
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* BannerID = dict[@"BannerID"];

        [self initFBAdsBanner:BannerID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_initFBAdsBanner];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* InterstitialID = dict[@"InterstitialID"];

        [self initFBAdsInterstitial:InterstitialID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_initFBAdsInterstitial];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* NativeID = dict[@"NativeID"];
        NSString* layout = dict[@"layout"];

        [self initFBAdsNativeAds:NativeID layout:layout];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_initFBAdsNativeAds];

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

        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* adsID = dict[@"adsID"];

        [self hideNativeAd:adsID];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_hideNativeAd];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* adsID = dict[@"adsID"];
        int width = [dict[@"width"] intValue];
        int height = [dict[@"height"] intValue];
        int x = [dict[@"x"] intValue];
        int y = [dict[@"y"] intValue];

        [self showNativeAd:adsID width:width height:height x:x y:y];

        return [EEDictionaryUtils emptyResult];
    } tag:k__facebookads_showNativeAd];

    [bridge registerHandler:^(NSString* msg) {
        return ([self hasInterstitialAd]) ? @"true" : @"false";
    } tag:k__facebookads_hasInterstitialAd];

    [bridge registerHandler:^(NSString* msg) {
        return ([self hasRewardedAd]) ? @"true" : @"false";
    } tag:k__facebookads_hasRewardedAd];

    [bridge registerHandler:^(NSString* msg) {
        NSDictionary* dict = [EEJsonUtils convertStringToDictionary:msg];

        NSString* adsID = dict[@"adsID"];

        return ([self hasNativeAd:adsID]) ? @"true" : @"false";
    } tag:k__facebookads_hasNativeAd];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__facebookads_initFBAdsInterstitial];
    [bridge deregisterHandler:k__facebookads_initFBAdsNativeAds];
    [bridge deregisterHandler:k__facebookads_initFBAdsBanner];
}

#pragma mark - CODE HERE

- (void)initTestDevice {
    // test device
    //    [FBAdSettings addTestDevice:[FBAdSettings testDeviceHash]];
    [FBAdSettings clearTestDevice:[FBAdSettings testDeviceHash]];
}

- (void)initFBAdsInterstitial:(NSString*)InterstitialID {
    _interstitialID = InterstitialID;
    _canShowInterstitialAd = NO;

    [self initTestDevice];

    // inter ad
    self.interstitialAd =
        [[FBInterstitialAd alloc] initWithPlacementID:InterstitialID];
    self.interstitialAd.delegate = self;
    [self.interstitialAd loadAd];
}

- (void)initFBAdsNativeAds:(NSString*)NativeID layout:(NSString*)layout {
    [self initTestDevice];

    if (!_dictFBNativeAd) {
        _dictFBNativeAd = [[NSMutableDictionary alloc] init];
        _dictFBNativeAdView = [[NSMutableDictionary alloc] init];
        _dictFBNativeAdReady = [[NSMutableDictionary alloc] init];
    }

    FBNativeAd* nativeAd = [[FBNativeAd alloc] initWithPlacementID:NativeID];
    nativeAd.delegate = self;
    nativeAd.mediaCachePolicy = FBNativeAdsCachePolicyAll;
    [nativeAd loadAd];

    [_dictFBNativeAd setObject:nativeAd forKey:NativeID];

    UIViewController* root =
        [[[UIApplication sharedApplication] keyWindow] rootViewController];

    EEFacebookAdsUIView* _nativeAdView =
        [[[NSBundle mainBundle] loadNibNamed:layout
                                       owner:nil
                                     options:nil] firstObject];
    _nativeAdView.hidden = YES;
    [root.view addSubview:_nativeAdView];

    [_dictFBNativeAdView setObject:_nativeAdView forKey:NativeID];
    [_dictFBNativeAdReady setValue:[NSNumber numberWithBool:NO]
                            forKey:NativeID];
}

- (void)initFBAdsBanner:(NSString*)BannerID {
    // not implement yet
}

- (void)cacheRewardedAd:(NSString*)adsID {
}

- (void)cacheInterstitialAd:(NSString*)adsID {
    [self initFBAdsInterstitial:adsID];
}

- (void)showBannerAd:(NSString*)adsID pos:(int)pos {
}

- (void)hideBannerAd {
}

- (bool)hasInterstitialAd {
    return _canShowInterstitialAd;
}

- (bool)hasRewardedAd {
    return false;
}

- (void)showInterstitialAd {
    NSLog(@"FBAds ======== showInterstitialAd");
    if (_canShowInterstitialAd) {
        UIViewController* root =
            [[[UIApplication sharedApplication] keyWindow] rootViewController];

        [self.interstitialAd showAdFromRootViewController:root];

        _canShowInterstitialAd = NO;
    } else {
        NSLog(@"FBAds ======== can not showInterstitialAd");
    }
}

- (void)showRewardedAd {
}

- (void)hideNativeAd:(NSString*)adsID {
    EEFacebookAdsUIView* _nativeAdView =
        [_dictFBNativeAdView objectForKey:adsID];
    _nativeAdView.hidden = YES;
}

- (BOOL)hasNativeAd:(NSString*)adsID {
    return [[_dictFBNativeAdReady valueForKey:adsID] boolValue];
}

- (void)showNativeAd:(NSString*)adsID
               width:(int)width
              height:(int)height
                   x:(int)x
                   y:(int)y {
    EEFacebookAdsUIView* _nativeAdView =
        [_dictFBNativeAdView objectForKey:adsID];

    if ([[_dictFBNativeAdReady valueForKey:adsID] boolValue]) {
        _nativeAdView.hidden = NO;
    }

    CGFloat scale = [[UIScreen mainScreen] scale];

    _nativeAdView.frame =
        CGRectMake((float)x / scale, (float)y / scale, width, height);
    _nativeAdView.adchoicesView.corner = UIRectCornerTopRight;

    // for custom view need call drawRect again
    [_nativeAdView.subviews[0] setNeedsDisplay];
    [_nativeAdView.subviews[1] setNeedsDisplay];
    [_nativeAdView.subviews[2] setNeedsDisplay];
}

- (void)onAdsCallback:(int)code msg:(NSString*)msg {
}

#pragma mark - FBInterstitialAdDelegate

- (void)interstitialAdDidClick:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdDidClick");
    [[EEMessageBridge getInstance] callCpp:k__facebookads_callback
                                       msg:@"interstitialAdDidClick"];
}

- (void)interstitialAdDidClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdDidClose");
    [[EEMessageBridge getInstance] callCpp:k__facebookads_callback
                                       msg:@"interstitialAdDidClose"];
    // reinit interstitial ads
    //    [self initFBAdsInterstitial:_interstitialID];
}

- (void)interstitialAdWillClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdWillClose");
}

- (void)interstitialAdDidLoad:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdDidLoad");

    _canShowInterstitialAd = YES;

    [[EEMessageBridge getInstance] callCpp:k__facebookads_callback
                                       msg:@"interstitialAdDidLoad"];
}
- (void)interstitialAd:(FBInterstitialAd*)interstitialAd
      didFailWithError:(NSError*)error {
    NSLog(@"FBAds ======== interstitialAd didFailWithError %@",
          error.description);
    [[EEMessageBridge getInstance] callCpp:k__facebookads_callback
                                       msg:@"interstitialAd didFailWithError"];
}
- (void)interstitialAdWillLogImpression:(FBInterstitialAd*)interstitialAd {
    NSLog(@"FBAds ======== interstitialAdWillLogImpression");
    [[EEMessageBridge getInstance] callCpp:k__facebookads_callback
                                       msg:@"interstitialAdWillLogImpression"];
}

#pragma mark - FBNativeAdDelegate

- (void)nativeAdDidLoad:(FBNativeAd*)nativeAd {
    NSLog(@"FBAds ======== nativeAd nativeAdDidLoad");
    UIViewController* root =
        [[[UIApplication sharedApplication] keyWindow] rootViewController];

    FBNativeAd* _nativeAd = [_dictFBNativeAd objectForKey:nativeAd.placementID];
    EEFacebookAdsUIView* _nativeAdView =
        [_dictFBNativeAdView objectForKey:nativeAd.placementID];

    if (_nativeAdView == nil) {
        return;
    }

    [_dictFBNativeAdReady setValue:[NSNumber numberWithBool:YES]
                            forKey:nativeAd.placementID];

    [_nativeAd unregisterView];

    // Wire up UIView with the native ad; the whole UIView will be clickable.
    [_nativeAd registerViewForInteraction:_nativeAdView
                       withViewController:root];

    // Create native UI using the ad metadata.
    //[_nativeAdView.mediaView setNativeAd:nativeAd];
    [_nativeAd.coverImage loadImageAsyncWithBlock:^(UIImage* image) {
        _nativeAdView.coverImg.image = image;
    }];

    [_nativeAd.icon loadImageAsyncWithBlock:^(UIImage* image) {
        _nativeAdView.iconImg.image = image;
    }];

    // Render native ads onto UIView
    _nativeAdView.titleLbl.text = _nativeAd.title;
    _nativeAdView.bodyLbl.text = _nativeAd.body;
    _nativeAdView.socialContext.text = _nativeAd.socialContext;
    _nativeAdView.sponsorLbl.text = @"Sponsored";
    [_nativeAdView.callToAction setTitle:_nativeAd.callToAction
                                forState:UIControlStateNormal];
    _nativeAdView.adchoicesView.nativeAd = nativeAd;
}

- (void)nativeAd:(FBNativeAd*)nativeAd didFailWithError:(NSError*)error {
    NSLog(@"FBAds ======== nativeAd didFailWithError");
    [_dictFBNativeAdReady setValue:[NSNumber numberWithBool:NO]
                            forKey:nativeAd.placementID];
}

#pragma mark - FBMediaViewDelegate

- (void)mediaViewDidLoad:(FBMediaView*)mediaView {
    NSLog(@"FBAds ======== mediaViewDidLoad");
}

@end
