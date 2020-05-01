//
//  EEAdMobBannerAd.m
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/admob/private/EEAdMobBannerAd.h"

#import <GoogleMobileAds/GADBannerView.h>

#import <ee/ads/internal/EEAdViewHelper.h>
#import <ee/ads/internal/EEMessageHelper.h>
#import <ee/ads/internal/EEViewHelper.h>
#import <ee/core/internal/EEIMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

@interface EEAdMobBannerAd () <GADBannerViewDelegate>
@end

@implementation EEAdMobBannerAd {
    id<EEIMessageBridge> bridge_;
    NSString* adId_;
    GADAdSize adSize_;
    EEMessageHelper* messageHelper_;
    EEAdViewHelper* helper_;
    EEViewHelper* viewHelper_;
    BOOL isLoaded_;
    GADBannerView* ad_;
}

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(GADAdSize)adSize {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    adId_ = [adId copy];
    adSize_ = adSize;
    messageHelper_ =
        [[EEMessageHelper alloc] initWithPrefix:@"AdMobBannerAd" adId:adId];
    helper_ = [[EEAdViewHelper alloc] initWithBridge:bridge_
                                                view:self
                                              helper:messageHelper_];
    [self createInternalAd];
    [self registerHandlers];
    return self;
}

- (void)destroy {
    NSAssert([EEUtils isMainThread], @"");
    [self deregisterhandlers];
    [self destroyInternalAd];

    [adId_ release];
    adId_ = nil;
    [messageHelper_ release];
    messageHelper_ = nil;
    [helper_ release];
    helper_ = nil;
}

- (void)dealloc {
    [super dealloc];
}

- (void)registerHandlers {
    [helper_ registerHandlers];
}

- (void)deregisterhandlers {
    [helper_ deregisterHandlers];
}

- (BOOL)createInternalAd {
    NSAssert([EEUtils isMainThread], @"");
    if (ad_ != nil) {
        return NO;
    }
    isLoaded_ = NO;

    GADBannerView* ad =
        [[[GADBannerView alloc] initWithAdSize:adSize_] autorelease];
    [ad setAdUnitID:adId_];
    [ad setDelegate:self];

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [ad setRootViewController:rootView];
    [[rootView view] addSubview:ad];

    ad_ = [ad retain];
    viewHelper_ = [[EEViewHelper alloc] initWithView:ad_];
    return YES;
}

- (BOOL)destroyInternalAd {
    NSAssert([EEUtils isMainThread], @"");
    if (ad_ == nil) {
        return NO;
    }
    isLoaded_ = NO;
    [viewHelper_ release];
    viewHelper_ = nil;
    [ad_ setDelegate:nil];
    [ad_ removeFromSuperview];
    [ad_ release];
    ad_ = nil;
    return YES;
}

- (BOOL)isLoaded {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    return isLoaded_;
}

- (void)load {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    GADRequest* request = [GADRequest request];
    [ad_ loadRequest:request];
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
    NSAssert(ad_ == bannerView, @"");
    isLoaded_ = YES;
    [bridge_ callCpp:[messageHelper_ onLoaded]];
}

- (void)adView:(GADBannerView*)bannerView
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(ad_ == bannerView, @"");
    [bridge_ callCpp:[messageHelper_ onFailedToLoad]
             message:[error description]];
}

- (void)adViewWillPresentScreen:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == bannerView, @"");
}

- (void)adViewWillDismissScreen:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == bannerView, @"");
}

- (void)adViewDidDismissScreen:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == bannerView, @"");
}

- (void)adViewWillLeaveApplication:(GADBannerView*)bannerView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == bannerView, @"");
    [bridge_ callCpp:[messageHelper_ onClicked]];
}

@end
