//
//  EEFacebookNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <FBAudienceNetwork/FBAudienceNetwork.h>

#import "ee/ads/internal/EEAdViewHelper.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebook/EEFacebookNativeAdView.h"
#import "ee/facebook/internal/EEFacebookNativeAd.h"

@interface EEFacebookNativeAd () <FBNativeAdDelegate> {
    FBNativeAd* nativeAd_;
    EEFacebookNativeAdView* nativeAdView_;
    NSString* adId_;
    NSString* layoutName_;
    BOOL isAdLoaded_;
    EEAdViewHelper* helper_;
}

@end

@implementation EEFacebookNativeAd

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
    helper_ =
        [[EEAdViewHelper alloc] initWithPrefix:@"FacebookNativeAd" adId:adId_];

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
    return
        [@"FacebookNativeAd_createInternalAd_" stringByAppendingString:adId_];
}

- (NSString* _Nonnull)k__destroyInternalAd {
    return
        [@"FacebookNativeAd_destroyInternalAd_" stringByAppendingString:adId_];
}

- (NSString* _Nonnull)k__onLoaded {
    return [@"FacebookNativeAd_onLoaded_" stringByAppendingString:adId_];
}

- (NSString* _Nonnull)k__onFailedToLoad {
    return [@"FacebookNativeAd_onFailedToLoad_" stringByAppendingString:adId_];
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
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGPoint position = [nativeAdView_ frame].origin;
    return CGPointMake(position.x * scale, position.y * scale);
}

- (void)setPosition:(CGPoint)position {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [nativeAdView_ frame];
    frame.origin = CGPointMake(position.x / scale, position.y / scale);
    [nativeAdView_ setFrame:frame];
}

- (CGSize)getSize {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGSize size = [nativeAdView_ frame].size;
    return CGSizeMake(size.width * scale, size.height * scale);
}

- (void)setSize:(CGSize)size {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [nativeAdView_ frame];
    frame.size = CGSizeMake(size.width / scale, size.height / scale);
    [nativeAdView_ setFrame:frame];
}

- (void)setVisible:(BOOL)visible {
    [nativeAdView_ setHidden:!visible];
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
