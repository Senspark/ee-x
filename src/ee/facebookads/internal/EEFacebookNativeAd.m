//
//  EEFacebookNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <FBAudienceNetwork/FBAudienceNetwork.h>

#import "ee/ads/internal/EEAdViewHelper.h"
#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebookads/EEFacebookNativeAdView.h"
#import "ee/facebookads/internal/EEFacebookNativeAd.h"

@interface EEFacebookNativeAd () <FBNativeAdDelegate> {
    /// Internal Facebook ad.
    FBNativeAd* nativeAd_;

    /// Loaded from xib.
    EEFacebookNativeAdView* nativeAdView_;

    /// Ad ID Unit.
    NSString* adId_;

    /// Layout name (xib).
    NSString* layoutName_;

    /// Whether the ad is loaded.
    BOOL isAdLoaded_;

    /// Common helper.
    EEAdViewHelper* helper_;
}

@end

@implementation EEFacebookNativeAd

static NSString* const k__tag = @"FacebookNativeAd";

- (id)initWithAdId:(NSString*)adId layout:(NSString*)layoutName {
    self = [super init];
    if (self == nil) {
        return self;
    }

    isAdLoaded_ = NO;
    adId_ = [adId copy];
    layoutName_ = [layoutName copy];
    nativeAd_ = nil;
    nativeAdView_ = nil;
    helper_ = [[EEAdViewHelper alloc] initWithPrefix:k__tag adId:adId_];

    [self createInternalAd];
    [self createView];
    [self registerHandlers];
    return self;
}

- (void)destroy {
    [self deregisterhandlers];
    [self destroyInternalAd];
    [self destroyView];
}

- (void)dealloc {
    [helper_ release];
    helper_ = nil;
    [adId_ release];
    adId_ = nil;
    [layoutName_ release];
    layoutName_ = nil;
    [super dealloc];
}

- (NSString* _Nonnull)k__createInternalAd {
    return [NSString stringWithFormat:@"%@_createInternalAd_%@", k__tag, adId_];
}

- (NSString* _Nonnull)k__destroyInternalAd {
    return
        [NSString stringWithFormat:@"%@_destroyInternalAd_%@", k__tag, adId_];
}

- (NSString* _Nonnull)k__onLoaded {
    return [NSString stringWithFormat:@"%@_onLoaded_%@", k__tag, adId_];
}

- (NSString* _Nonnull)k__onFailedToLoad {
    return [NSString stringWithFormat:@"%@_onFailedToLoad_%@", k__tag, adId_];
}

- (void)registerHandlers {
    [helper_ registerHandlers:self];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge registerHandler:[self k__createInternalAd]
                   callback:^(NSString* message) {
                       return [EEUtils toString:[self createInternalAd]];
                   }];
    [bridge registerHandler:[self k__destroyInternalAd]
                   callback:^(NSString* message) {
                       return [EEUtils toString:[self destroyInternalAd]];
                   }];
}

- (void)deregisterhandlers {
    [helper_ deregisterHandlers];

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge deregisterHandler:[self k__createInternalAd]];
    [bridge deregisterHandler:[self k__destroyInternalAd]];
}

- (BOOL)createInternalAd {
    if (nativeAd_ != nil) {
        return NO;
    }
    isAdLoaded_ = NO;
    FBNativeAd* nativeAd =
        [[[FBNativeAd alloc] initWithPlacementID:adId_] autorelease];
    [nativeAd setDelegate:self];
    [nativeAd setMediaCachePolicy:FBNativeAdsCachePolicyAll];
    nativeAd_ = [nativeAd retain];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (nativeAd_ == nil) {
        return NO;
    }
    isAdLoaded_ = NO;
    [nativeAd_ setDelegate:nil];
    [nativeAd_ unregisterView];
    [nativeAd_ release];
    nativeAd_ = nil;
    return YES;
}

- (void)createView {
    NSAssert(nativeAdView_ == nil, @"");
    EEFacebookNativeAdView* adView =
        [[[NSBundle mainBundle] loadNibNamed:layoutName_ owner:nil options:nil]
            firstObject];
    [adView setHidden:YES];
    [[adView adchoicesView] setCorner:UIRectCornerTopRight];

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [[rootView view] addSubview:adView];

    nativeAdView_ = [adView retain];
}

- (void)destroyView {
    NSAssert(nativeAdView_ != nil, @"");
    [nativeAdView_ removeFromSuperview];
    [nativeAdView_ release];
    nativeAdView_ = nil;
}

- (BOOL)isLoaded {
    if (nativeAd_ == nil) {
        return NO;
    }
    return isAdLoaded_;
}

- (void)load {
    if (nativeAd_ == nil) {
        return;
    }
    [nativeAd_ loadAd];
}

- (CGPoint)getPosition {
    return [EEAdViewHelper getPosition:nativeAdView_];
}

- (void)setPosition:(CGPoint)position {
    [EEAdViewHelper setPosition:position for:nativeAdView_];
}

- (CGSize)getSize {
    return [EEAdViewHelper getSize:nativeAdView_];
}

- (void)setSize:(CGSize)size {
    [EEAdViewHelper setSize:size for:nativeAdView_];
}

- (void)setVisible:(BOOL)visible {
    [EEAdViewHelper setVisible:visible for:nativeAdView_];
    if (visible) {
        for (UIView* subView in [nativeAdView_ subviews]) {
            [subView setNeedsDisplay];
        }
    }
}

- (void)nativeAdDidLoad:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(nativeAd == nativeAd_, @"");

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [nativeAd unregisterView];
    [nativeAd registerViewForInteraction:nativeAdView_
                      withViewController:rootView
                      withClickableViews:@[[nativeAdView_ callToActionButton]]];

    [[nativeAd coverImage] loadImageAsyncWithBlock:^(UIImage* image) {
        [[nativeAdView_ coverImage] setImage:image];
    }];

    [[nativeAd icon] loadImageAsyncWithBlock:^(UIImage* image) {
        [[nativeAdView_ iconImage] setImage:image];
    }];

    [[nativeAdView_ adchoicesView] setNativeAd:nativeAd];
    [[nativeAdView_ bodyLabel] setText:[nativeAd body]];
    [[nativeAdView_ callToActionButton] setTitle:[nativeAd callToAction]
                                        forState:UIControlStateNormal];
    [[nativeAdView_ socialContextLabel] setText:[nativeAd socialContext]];
    [[nativeAdView_ sponsorLabel] setText:@"Sponsored"];
    [[nativeAdView_ titleLabel] setText:[nativeAd title]];

    isAdLoaded_ = YES;
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onLoaded]];
}

- (void)nativeAdWillLogImpression:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)nativeAd:(FBNativeAd*)nativeAd didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)nativeAdDidClick:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)nativeAdDidFinishHandlingClick:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
