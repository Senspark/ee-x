//
//  EEFacebookNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import "ee/facebook_ads/private/EEFacebookNativeAd.h"

#import <FBAudienceNetwork/FBAdChoicesView.h>
#import <FBAudienceNetwork/FBAdIconView.h>
#import <FBAudienceNetwork/FBMediaView.h>
#import <FBAudienceNetwork/FBNativeAd.h>

#import <ee/ads/internal/EEAdViewHelper.h>
#import <ee/ads/internal/EEMessageHelper.h>
#import <ee/ads/internal/EEViewHelper.h>
#import <ee/core/internal/EEIMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

#import "ee/facebook_ads/EEFacebookNativeAdView.h"

@interface EEFacebookNativeAd () <FBNativeAdDelegate, FBMediaViewDelegate>
@end

@implementation EEFacebookNativeAd {
    id<EEIMessageBridge> bridge_;

    /// Ad ID Unit.
    NSString* adId_;

    /// Layout name (xib).
    NSString* layoutName_;

    /// Identifiers
    NSDictionary* identifiers_;

    EEMessageHelper* messageHelper_;

    /// Common helper.
    EEAdViewHelper* helper_;

    EEViewHelper* viewHelper_;

    /// Whether the ad is loaded.
    BOOL isLoaded_;

    /// Internal Facebook ad.
    FBNativeAd* nativeAd_;

    /// Loaded from xib.
    EEFacebookNativeAdView* nativeAdView_;
}

// clang-format off
static NSString* const k__ad_choices        = @"ad_choices";
static NSString* const k__body              = @"body";
static NSString* const k__call_to_action    = @"call_to_action";
static NSString* const k__icon              = @"icon";
static NSString* const k__media             = @"media";
static NSString* const k__social_context    = @"social_context";
static NSString* const k__title             = @"title";
static NSString* const k__cover             = @"cover";
static NSString* const k__sponsor           = @"sponsor";
// clang-format on

- (id)initWithBridge:(id<EEIMessageBridge>)bridge
                adId:(NSString* _Nonnull)adId
              layout:(NSString* _Nonnull)layoutName
         identifiers:(NSDictionary* _Nonnull)identifiers {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    adId_ = [adId copy];
    layoutName_ = [layoutName copy];
    identifiers_ = [identifiers copy];
    messageHelper_ =
        [[EEMessageHelper alloc] initWithPrefix:@"FacebookNativeAd" adId:adId];
    helper_ = [[EEAdViewHelper alloc] initWithBridge:bridge_
                                                view:self
                                              helper:messageHelper_];
    [self createInternalAd];
    [self createView];
    [self registerHandlers];
    return self;
}

- (void)destroy {
    [self deregisterhandlers];
    [self destroyView];
    [self destroyInternalAd];

    [adId_ release];
    adId_ = nil;
    [layoutName_ release];
    layoutName_ = nil;
    [identifiers_ release];
    identifiers_ = nil;
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
    [bridge_ registerHandler:[messageHelper_ createInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self createInternalAd]];
                    }];
    [bridge_ registerHandler:[messageHelper_ destroyInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self destroyInternalAd]];
                    }];
}

- (void)deregisterhandlers {
    [helper_ deregisterHandlers];
    [bridge_ deregisterHandler:[messageHelper_ createInternalAd]];
    [bridge_ deregisterHandler:[messageHelper_ destroyInternalAd]];
}

- (BOOL)createInternalAd {
    if (nativeAd_ != nil) {
        return NO;
    }
    isLoaded_ = NO;
    FBNativeAd* nativeAd =
        [[[FBNativeAd alloc] initWithPlacementID:adId_] autorelease];
    [nativeAd setDelegate:self];
    //    [nativeAd setMediaCachePolicy:FBNativeAdsCachePolicyAll];
    nativeAd_ = [nativeAd retain];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (nativeAd_ == nil) {
        return NO;
    }
    isLoaded_ = NO;
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
    [adView adchoicesView].corner = UIRectCornerTopRight;

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [[rootView view] addSubview:adView];

    nativeAdView_ = [adView retain];
    viewHelper_ = [[EEViewHelper alloc] initWithView:nativeAdView_];
}

- (void)destroyView {
    NSAssert(nativeAdView_ != nil, @"");
    [viewHelper_ release];
    viewHelper_ = nil;
    [nativeAdView_ removeFromSuperview];
    [nativeAdView_ release];
    nativeAdView_ = nil;
}

- (BOOL)isLoaded {
    if (nativeAd_ == nil) {
        return NO;
    }
    return isLoaded_;
}

- (void)load {
    if (nativeAd_ == nil) {
        return;
    }
    [nativeAd_ loadAdWithMediaCachePolicy:FBNativeAdsCachePolicyAll];
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
    if (visible) {
        for (UIView* subView in [nativeAdView_ subviews]) {
            [subView setNeedsDisplay];
        }
    }
}

- (void)nativeAdDidLoad:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(nativeAd == nativeAd_, @"");

    [nativeAd unregisterView];

    if ([nativeAdView_ callToActionButton]) {
        UIViewController* rootView = [EEUtils getCurrentRootViewController];
        [nativeAd
            registerViewForInteraction:nativeAdView_
                             mediaView:nativeAdView_.mediaView
                              iconView:nativeAdView_.iconView
                        viewController:rootView
                        clickableViews:@[[nativeAdView_ callToActionButton]]];
    }

    // cover image
    //    [[nativeAd coverImage] loadImageAsyncWithBlock:^(UIImage* image) {
    //        [[nativeAdView_ coverImage] setImage:image];
    //    }];

    // ad icon
    //    [[nativeAd icon] loadImageAsyncWithBlock:^(UIImage* image) {
    //        [[nativeAdView_ iconImage] setImage:image];
    //        [nativeAd registerViewForInteraction:nativeAdView_
    //                          withViewController:rootView];
    //    }];

    // adchoices view
    [[nativeAdView_ adchoicesView] setNativeAd:nativeAd];

    // ad body view
    [[nativeAdView_ bodyLabel] setText:[nativeAd bodyText]];

    // ad call to action button
    [[nativeAdView_ callToActionButton] setTitle:[nativeAd callToAction]
                                        forState:UIControlStateNormal];

    // ad social context label
    [[nativeAdView_ socialContextLabel] setText:[nativeAd socialContext]];

    // ad sponsored view
    [[nativeAdView_ sponsorLabel] setText:[nativeAd sponsoredTranslation]];

    // ad title view
    [[nativeAdView_ titleLabel] setText:[nativeAd headline]];

    // ad media view
    [[nativeAdView_ mediaView] setDelegate:self];
    [[nativeAdView_ iconView] setDelegate:self];

    isLoaded_ = YES;
    [bridge_ callCpp:[messageHelper_ onLoaded]];
}

- (void)nativeAdWillLogImpression:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)nativeAd:(FBNativeAd*)nativeAd didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    [bridge_ callCpp:[messageHelper_ onFailedToLoad]
             message:[error description]];
}

- (void)nativeAdDidClick:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:[messageHelper_ onClicked]];
}

- (void)nativeAdDidFinishHandlingClick:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)mediaViewDidLoad:(FBMediaView*)mediaView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    // hidden cover image when media did load.
    //    [[nativeAdView_ coverImage] setHidden:[identifiers_
    //    objectForKey:k__media]];
}
@end
