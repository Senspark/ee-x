//
//  EEFacebookBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import "ee/facebook_ads/private/EEFacebookBannerAd.h"

#import <FBAudienceNetwork/FBAdView.h>

#import <ee/ads/internal/EEAdViewHelper.h>
#import <ee/ads/internal/EEViewHelper.h>
#import <ee/core/internal/EEIMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

@interface EEFacebookBannerAd () <FBAdViewDelegate> {
    id<EEIMessageBridge> bridge_;
    FBAdView* adView_;
    NSString* adId_;
    FBAdSize adSize_;
    BOOL isLoaded_;
    EEAdViewHelper* helper_;
    EEViewHelper* viewHelper_;
}

@end

@implementation EEFacebookBannerAd

+ (FBAdSize)adSizeFor:(int)index {
    if (index == 0) {
        return kFBAdSizeHeight50Banner;
    }
    if (index == 1) {
        return kFBAdSizeHeight90Banner;
    }
    if (index == 2) {
        return kFBAdSizeInterstitial;
    }
    if (index == 3) {
        return kFBAdSizeHeight250Rectangle;
    }
    if (index == 4) {
        return kFBAdSize320x50;
    }
    NSAssert(NO, @"");
    return kFBAdSize320x50;
}

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(FBAdSize)adSize {
    self = [super init];
    if (self == nil) {
        return self;
    }

    bridge_ = bridge;
    isLoaded_ = NO;
    adId_ = [adId copy];
    adSize_ = adSize;
    adView_ = nil;
    helper_ = [[EEAdViewHelper alloc] initWithBridge:bridge_
                                                view:self
                                              prefix:@"FacebookBannerAd"
                                                adId:adId_];

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
    return [@"FacebookBannerAd_onLoaded_" stringByAppendingString:adId_];
}

- (NSString* _Nonnull)k__onFailedToLoad {
    return [@"FacebookBannerAd_onFailedToLoad_" stringByAppendingString:adId_];
}

- (NSString* _Nonnull)k__onClicked {
    return [@"FacebookBannerAd_onClicked_" stringByAppendingString:adId_];
}

- (void)registerHandlers {
    [helper_ registerHandlers];
}

- (void)deregisterhandlers {
    [helper_ deregisterHandlers];
}

- (BOOL)createInternalAd {
    if (adView_ != nil) {
        return NO;
    }

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    FBAdView* adView =
        [[[FBAdView alloc] initWithPlacementID:adId_
                                        adSize:adSize_
                            rootViewController:rootView] autorelease];
    [adView setDelegate:self];

    [[rootView view] addSubview:adView];
    adView_ = [adView retain];
    viewHelper_ = [[EEViewHelper alloc] initWithView:adView_];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (adView_ == nil) {
        return NO;
    }
    isLoaded_ = NO;
    [viewHelper_ release];
    viewHelper_ = nil;
    [adView_ setDelegate:nil];
    [adView_ removeFromSuperview];
    [adView_ release];
    adView_ = nil;
    return YES;
}

- (BOOL)isLoaded {
    if (adView_ == nil) {
        return NO;
    }
    return isLoaded_;
}

- (void)load {
    [adView_ loadAd];
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

- (void)setVisible:(BOOL)visible {
    [viewHelper_ setVisible:visible];
}

- (void)adViewDidClick:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:[self k__onClicked]];
}

- (void)adViewDidFinishHandlingClick:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)adViewDidLoad:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    isLoaded_ = YES;
    [bridge_ callCpp:[self k__onLoaded]];
}

- (void)adView:(FBAdView*)adView didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    [bridge_ callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)adViewWillLogImpression:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
