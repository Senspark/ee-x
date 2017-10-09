//
//  EEFacebookAds.m
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <FBAudienceNetwork/FBAudienceNetwork.h>

#import "ee/core/internal/EEDictionaryUtils.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebook/EEFacebookAds.h"
#import "ee/facebook/internal/EEFacebookBannerAd.h"
#import "ee/facebook/internal/EEFacebookNativeAd.h"

@interface EEFacebookAds () <FBAdViewDelegate, FBInterstitialAdDelegate,
                             FBNativeAdDelegate, FBMediaViewDelegate> {
    NSMutableDictionary<NSString*, EEFacebookBannerAd*>* bannerAds_;
    NSMutableDictionary<NSString*, EEFacebookNativeAd*>* nativeAds_;
}

@property (nonatomic, retain) FBInterstitialAd* _Nullable interstitialAd;

@end

@implementation EEFacebookAds

// clang-format off
static NSString* const k__createBannerAd       = @"FacebookAds_createBannerAd";
static NSString* const k__destroyBannerAd      = @"FacebookAds_destroyBannerAd";

static NSString* const k__createNativeAd       = @"FacebookAds_createNativeAd";
static NSString* const k__destroyNativeAd      = @"FacebookAds_destroyNativeAd";

static NSString* const k__cacheInterstitialAd  = @"FacebookAds_cacheInterstitialAd";
static NSString* const k__showInterstitialAd   = @"FacebookAds_showInterstitialAd";

static NSString* const k__cppCallback          = @"FacebookAds_cppCallback";
// clang-format on

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    [self registerHandlers];
    bannerAds_ = [[NSMutableDictionary alloc] init];
    nativeAds_ = [[NSMutableDictionary alloc] init];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [self setInterstitialAd:nil];
    [bannerAds_ release];
    bannerAds_ = nil;
    [nativeAds_ release];
    nativeAds_ = nil;
    [super dealloc];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:k__createBannerAd
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* adId = dict[@"adId"];
                       int adSizeIndex = [dict[@"adSize"] intValue];
                       FBAdSize adSize =
                           [EEFacebookBannerAd adSizeFor:adSizeIndex];
                       return [self createBannerAd:adId size:adSize] ? @"true"
                                                                     : @"false";
                   }];

    [bridge registerHandler:k__destroyBannerAd
                   callback:^(NSString* message) {
                       NSString* adId = message;
                       return [self destroyBannerAd:adId] ? @"true" : @"false";
                   }];

    [bridge registerHandler:k__createNativeAd
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       NSString* adId = dict[@"adId"];
                       NSString* layout = dict[@"layout"];
                       return [self createNativeAd:adId layout:layout]
                                  ? @"true"
                                  : @"false";
                   }];

    [bridge registerHandler:k__destroyNativeAd
                   callback:^(NSString* message) {
                       NSString* adId = message;
                       return [self destroyNativeAd:adId] ? @"true" : @"false";
                   }];

    [bridge registerHandler:k__cacheInterstitialAd
                   callback:^(NSString* message) {
                       NSString* adId = message;
                       [self cacheInterstitialAd:adId];
                       return @"";
                   }];

    [bridge registerHandler:k__showInterstitialAd
                   callback:^(NSString* message) {
                       return [self showInterstitialAd] ? @"true" : @"false";
                   }];
}

- (void)deregisterHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:k__createBannerAd];
    [bridge deregisterHandler:k__destroyBannerAd];

    [bridge deregisterHandler:k__createNativeAd];
    [bridge deregisterHandler:k__destroyNativeAd];

    [bridge deregisterHandler:k__cacheInterstitialAd];
    [bridge deregisterHandler:k__showInterstitialAd];
}

#pragma mark - CODE HERE

- (void)initTestDevice {
    // test device
    //    [FBAdSettings addTestDevice:[FBAdSettings testDeviceHash]];
    //    [FBAdSettings clearTestDevice:[FBAdSettings testDeviceHash]];
}

- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(FBAdSize)size {
    if ([bannerAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEFacebookBannerAd* ad =
        [[[EEFacebookBannerAd alloc] initWithAdId:adId size:size] autorelease];
    [bannerAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyBannerAd:(NSString* _Nonnull)adId {
    if ([bannerAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEFacebookBannerAd* ad = [bannerAds_ objectForKey:adId];
    [ad release];
    [bannerAds_ removeObjectForKey:adId];
    return YES;
}

- (BOOL)createNativeAd:(NSString*)adId layout:(NSString*)layout {
    if ([nativeAds_ objectForKey:adId] != nil) {
        return NO;
    }
    EEFacebookNativeAd* ad =
        [[[EEFacebookNativeAd alloc] initWithAdId:adId layout:layout]
            autorelease];
    [nativeAds_ setObject:ad forKey:adId];
    return YES;
}

- (BOOL)destroyNativeAd:(NSString*)adId {
    if ([nativeAds_ objectForKey:adId] == nil) {
        return NO;
    }
    EEFacebookNativeAd* ad = [nativeAds_ objectForKey:adId];
    [ad release];
    [nativeAds_ removeObjectForKey:adId];
    return YES;
}

- (void)showNativeAd:(NSString*)adId
                size:(CGSize)size
            position:(CGPoint)position {
    /*
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
     */
}

- (FBInterstitialAd* _Nullable)createInterstitialAd:(NSString* _Nonnull)adId {
    FBInterstitialAd* ad =
        [[[FBInterstitialAd alloc] initWithPlacementID:adId] autorelease];
    [ad setDelegate:self];
    [ad loadAd];
    return ad;
}

- (void)cacheInterstitialAd:(NSString* _Nonnull)adId {
    if ([self interstitialAd] == nil) {
        [self setInterstitialAd:[self createInterstitialAd:adId]];
        return;
    }
    if ([[[self interstitialAd] placementID] isEqualToString:adId]) {
        [[self interstitialAd] loadAd];
    } else {
        [self setInterstitialAd:[self createInterstitialAd:adId]];
    }
}

- (bool)hasInterstitialAd {
    if ([self interstitialAd] == nil) {
        return NO;
    }
    return [[self interstitialAd] isAdValid];
}

- (BOOL)showInterstitialAd {
    if (![self hasInterstitialAd]) {
        return NO;
    }
    UIViewController* view = [EEUtils getCurrentRootViewController];
    return [[self interstitialAd] showAdFromRootViewController:view];
}

- (void)onAdsCallback:(int)code msg:(NSString*)msg {
}

#pragma mark - FBInterstitialAdDelegate

- (void)interstitialAdDidClick:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)interstitialAdDidClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self cacheInterstitialAd:[interstitialAd placementID]];
}

- (void)interstitialAdWillClose:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)interstitialAdDidLoad:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)interstitialAd:(FBInterstitialAd*)interstitialAd
      didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
}

- (void)interstitialAdWillLogImpression:(FBInterstitialAd*)interstitialAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

#pragma mark - FBMediaViewDelegate

- (void)mediaViewDidLoad:(FBMediaView*)mediaView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)mediaViewVideoDidComplete:(FBMediaView*)mediaView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
