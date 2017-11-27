//
//  EEFacebookBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <FBAudienceNetwork/FBAdView.h>

#import "ee/ads/internal/EEAdViewHelper.h"
#import "ee/core/EEMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebook/internal/EEFacebookBannerAd.h"

@interface EEFacebookBannerAd () <FBAdViewDelegate> {
    FBAdView* adView_;
    NSString* adId_;
    FBAdSize adSize_;
    BOOL isLoaded_;
    EEAdViewHelper* helper_;
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
    NSAssert(NO, @"");
    return kFBAdSize320x50;
}

- (id)initWithAdId:(NSString* _Nonnull)adId size:(FBAdSize)adSize {
    self = [super init];
    if (self == nil) {
        return self;
    }

    isLoaded_ = NO;
    adId_ = [adId copy];
    adSize_ = adSize;
    adView_ = nil;
    helper_ =
        [[EEAdViewHelper alloc] initWithPrefix:@"FacebookBannerAd" adId:adId_];

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

- (void)registerHandlers {
    [helper_ registerHandlers:self];
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
    adView_ = adView;
    return YES;
}

- (BOOL)destroyInternalAd {
    if (adView_ == nil) {
        return NO;
    }
    isLoaded_ = NO;
    [adView_ removeFromSuperview];
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
    return [EEAdViewHelper getPosition:adView_];
}

- (void)setPosition:(CGPoint)position {
    [EEAdViewHelper setPosition:position for:adView_];
}

- (CGSize)getSize {
    return [EEAdViewHelper getSize:adView_];
}

- (void)setSize:(CGSize)size {
    [EEAdViewHelper setSize:size for:adView_];
}

- (void)setVisible:(BOOL)visible {
    [EEAdViewHelper setVisible:visible for:adView_];
}

- (void)adViewDidClick:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)adViewDidFinishHandlingClick:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)adViewDidLoad:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    isLoaded_ = YES;

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onLoaded]];
}

- (void)adView:(FBAdView*)adView didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)adViewWillLogImpression:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
