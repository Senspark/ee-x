//
//  EEAdMobBannerAd.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/admob/private/EEAdMobBannerAd.h"

#import <GoogleMobileAds/GoogleMobileAds.h>

#import <ee/ads/internal/EEAdViewHelper.h>
#import <ee/ads/internal/EEViewHelper.h>
#import <ee/core/internal/EEIMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

@interface EEAdMobBannerAd () <GADBannerViewDelegate> {
    id<EEIMessageBridge> bridge_;
    BOOL isLoaded_;
    GADBannerView* bannerView_;
    GADAdSize adSize_;
    NSString* adId_;
    EEAdViewHelper* helper_;
    EEViewHelper* viewHelper_;
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
    if (index == 3) {
        return kGADAdSizeMediumRectangle;
    }
    return kGADAdSizeInvalid;
}

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(GADAdSize)adSize {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    isLoaded_ = NO;
    bannerView_ = nil;
    adId_ = [adId copy];
    adSize_ = adSize;
    helper_ = [[EEAdViewHelper alloc] initWithBridge:bridge_
                                                view:self
                                              prefix:@"AdMobBannerAd"
                                                adId:adId];

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

- (NSString* _Nonnull)k__onLoaded {
    return [@"AdMobBannerAd_onLoaded_" stringByAppendingString:adId_];
}

- (NSString* _Nonnull)k__onFailedToLoad {
    return [@"AdMobBannerAd_onFailedToLoad_" stringByAppendingString:adId_];
}

- (NSString* _Nonnull)k__onClicked {
    return [@"AdMobBannerAd_onClicked_" stringByAppendingString:adId_];
}

- (void)registerHandlers {
    [helper_ registerHandlers];
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
    viewHelper_ = [[EEViewHelper alloc] initWithView:bannerView_];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (bannerView_ == nil) {
        return NO;
    }
    isLoaded_ = NO;
    [viewHelper_ release];
    viewHelper_ = nil;
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
    return [viewHelper_ getPosition];
}

- (void)setPosition:(CGPoint)position {
    [viewHelper_ setPosition:position];
}

- (CGSize)getSize {
    return [viewHelper_ getSize];
}

- (void)setSize:(CGSize)size {
    [viewHelper_ setSize:size];
}

- (BOOL)isVisible {
    return [viewHelper_ isVisible];
}

- (void)setVisible:(BOOL)visible {
    [viewHelper_ setVisible:visible];
}

- (void)adViewDidReceiveAd:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(bannerView_ == bannerView, @"");
    [bridge_ callCpp:[self k__onLoaded]];
}

- (void)adView:(GADBannerView*)bannerView
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(bannerView_ == bannerView, @"");
    [bridge_ callCpp:[self k__onFailedToLoad] message:[error description]];
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
    [bridge_ callCpp:[self k__onClicked]];
}

@end
