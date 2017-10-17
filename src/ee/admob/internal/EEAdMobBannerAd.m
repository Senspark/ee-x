//
//  EEAdMobBannerAd.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <GoogleMobileAds/GoogleMobileAds.h>

#import "ee/admob/internal/EEAdMobBannerAd.h"
#import "ee/ads/internal/EEAdViewHelper.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"

@interface EEAdMobBannerAd () <GADBannerViewDelegate> {
    BOOL isLoaded_;
    GADBannerView* bannerView_;
    GADAdSize adSize_;
    NSString* adId_;
    EEAdViewHelper* helper_;
}

@end

@implementation EEAdMobBannerAd

+ (GADAdSize)adSizeFor:(int)index {
    if (index == 0) {
        return kGADAdSizeBanner;
    }
    if (index == 1) {
        return kGADAdSizeLargeBanner;
    }
    if (index == 2) {
        if ([EEUtils isLandscape]) {
            return kGADAdSizeSmartBannerLandscape;
        }
        return kGADAdSizeSmartBannerPortrait;
    }
    return kGADAdSizeInvalid;
}

- (id _Nonnull)initWithAdId:(NSString* _Nonnull)adId size:(GADAdSize)adSize {
    self = [super init];
    if (self == nil) {
        return self;
    }
    isLoaded_ = NO;
    bannerView_ = nil;
    adId_ = [adId copy];
    adSize_ = adSize;
    helper_ =
        [[EEAdViewHelper alloc] initWithPrefix:@"AdMobBannerAd" adId:adId];

    [self createInternalAd];
    [self registerHandlers];
    return self;
}

- (void)destroy {
    [self deregisterhandlers];
    [self destroyInternalAd];
}

- (void)dealloc {
    [helper_ release];
    helper_ = nil;
    [adId_ release];
    adId_ = nil;
    [super dealloc];
}

- (void)registerHandlers {
    [helper_ registerHandlers:self];
}

- (void)deregisterhandlers {
    [helper_ deregisterHandlers];
}

- (BOOL)createInternalAd {
    if (bannerView_ != nil) {
        return NO;
    }
    isLoaded_ = NO;

    GADBannerView* bannerView =
        [[[GADBannerView alloc] initWithAdSize:adSize_] autorelease];
    [bannerView setAdUnitID:adId_];
    [bannerView setDelegate:self];

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [bannerView setRootViewController:rootView];
    [[rootView view] addSubview:bannerView];

    bannerView_ = [bannerView retain];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (bannerView_ == nil) {
        return NO;
    }
    isLoaded_ = NO;
    [bannerView_ setDelegate:nil];
    [bannerView_ removeFromSuperview];
    [bannerView_ release];
    bannerView_ = nil;
    return YES;
}

- (BOOL)isLoaded {
    if (bannerView_ == nil) {
        return NO;
    }
    return isLoaded_;
}

- (void)load {
    if (bannerView_ == nil) {
        return;
    }
    GADRequest* request = [GADRequest request];
    [bannerView_ loadRequest:request];
}

- (CGPoint)getPosition {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGPoint position = [bannerView_ frame].origin;
    return CGPointMake(position.x * scale, position.y * scale);
}

- (void)setPosition:(CGPoint)position {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [bannerView_ frame];
    frame.origin = CGPointMake(position.x / scale, position.y / scale);
    [bannerView_ setFrame:frame];
}

- (CGSize)getSize {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGSize size = [bannerView_ frame].size;
    return CGSizeMake(size.width * scale, size.height * scale);
}

- (void)setSize:(CGSize)size {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [bannerView_ frame];
    frame.size = CGSizeMake(size.width / scale, size.height / scale);
    [bannerView_ setFrame:frame];
}

- (void)setVisible:(BOOL)visible {
    [bannerView_ setHidden:!visible];
}

- (void)adViewDidReceiveAd:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(bannerView_ == bannerView, @"");
}

- (void)adView:(GADBannerView*)bannerView
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(bannerView_ == bannerView, @"");
}

- (void)adViewWillPresentScreen:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(bannerView_ == bannerView, @"");
}

- (void)adViewWillDismissScreen:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(bannerView_ == bannerView, @"");
}

- (void)adViewDidDismissScreen:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(bannerView_ == bannerView, @"");
}

- (void)adViewWillLeaveApplication:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(bannerView_ == bannerView, @"");
}

@end
