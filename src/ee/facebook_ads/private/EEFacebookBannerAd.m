//
//  EEFacebookBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import "ee/facebook_ads/private/EEFacebookBannerAd.h"

#import <FBAudienceNetwork/FBAdView.h>

#import <ee_x-Swift.h>

#import <ee/ads/internal/EEAdViewHelper.h>
#import <ee/ads/internal/EEMessageHelper.h>
#import <ee/ads/internal/EEViewHelper.h>

@interface EEFacebookBannerAd () <FBAdViewDelegate>
@end

@implementation EEFacebookBannerAd {
    id<EEIMessageBridge> bridge_;
    NSString* adId_;
    FBAdSize adSize_;
    EEMessageHelper* messageHelper_;
    EEAdViewHelper* helper_;
    EEViewHelper* viewHelper_;
    BOOL isLoaded_;
    FBAdView* ad_;
}

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(FBAdSize)adSize {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }

    bridge_ = bridge;
    adId_ = [adId copy];
    adSize_ = adSize;
    messageHelper_ =
        [[EEMessageHelper alloc] initWithPrefix:@"FacebookBannerAd" adId:adId];
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

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    FBAdView* ad =
        [[[FBAdView alloc] initWithPlacementID:adId_
                                        adSize:adSize_
                            rootViewController:rootView] autorelease];
    [ad setDelegate:self];
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
    [ad_ loadAd];
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

- (void)adViewDidClick:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == adView, @"");
    [bridge_ callCpp:[messageHelper_ onClicked]];
}

- (void)adViewDidFinishHandlingClick:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == adView, @"");
}

- (void)adViewDidLoad:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == adView, @"");
    isLoaded_ = YES;
    [bridge_ callCpp:[messageHelper_ onLoaded]];
}

- (void)adView:(FBAdView*)adView didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(ad_ == adView, @"");
    [bridge_ callCpp:[messageHelper_ onFailedToLoad] //
                    :[error description]];
}

- (void)adViewWillLogImpression:(FBAdView*)adView {
    NSAssert(ad_ == adView, @"");
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
