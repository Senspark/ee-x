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

#import <ee_x-Swift.h>

#import <ee/ads/internal/EEAdViewHelper.h>
#import <ee/ads/internal/EEMessageHelper.h>
#import <ee/ads/internal/EEViewHelper.h>
#import <ee/core/internal/EEIMessageBridge.h>

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
    FBNativeAd* ad_;

    /// Loaded from xib.
    EEFacebookNativeAdView* view_;
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
    NSAssert([EEUtils isMainThread], @"");
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
    NSAssert([EEUtils isMainThread], @"");
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
    NSAssert([EEUtils isMainThread], @"");
    if (ad_ != nil) {
        return NO;
    }
    isLoaded_ = NO;
    ad_ = [[FBNativeAd alloc] initWithPlacementID:adId_];
    [ad_ setDelegate:self];
    return YES;
}

- (BOOL)destroyInternalAd {
    NSAssert([EEUtils isMainThread], @"");
    if (ad_ == nil) {
        return NO;
    }
    isLoaded_ = NO;
    [ad_ setDelegate:nil];
    [ad_ unregisterView];
    [ad_ release];
    ad_ = nil;
    return YES;
}

- (void)createView {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(view_ == nil, @"");
    EEFacebookNativeAdView* view =
        [[[NSBundle mainBundle] loadNibNamed:layoutName_ owner:nil options:nil]
            firstObject];
    [view setHidden:YES];
    [view adchoicesView].corner = UIRectCornerTopRight;

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [[rootView view] addSubview:view];

    view_ = [view retain];
    viewHelper_ = [[EEViewHelper alloc] initWithView:view_];
}

- (void)destroyView {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(view_ != nil, @"");
    [viewHelper_ release];
    viewHelper_ = nil;
    [view_ removeFromSuperview];
    [view_ release];
    view_ = nil;
}

- (BOOL)isLoaded {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    return isLoaded_;
}

- (void)load {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(ad_ != nil, @"");
    [ad_ loadAdWithMediaCachePolicy:FBNativeAdsCachePolicyAll];
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
        for (UIView* subView in [view_ subviews]) {
            [subView setNeedsDisplay];
        }
    }
}

- (void)nativeAdDidLoad:(FBNativeAd*)ad {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad == ad_, @"");

    [ad unregisterView];

    if ([view_ callToActionButton]) {
        UIViewController* rootView = [EEUtils getCurrentRootViewController];
        [ad registerViewForInteraction:view_
                             mediaView:view_.mediaView
                              iconView:view_.iconView
                        viewController:rootView
                        clickableViews:@[[view_ callToActionButton]]];
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
    [[view_ adchoicesView] setNativeAd:ad];

    // ad body view
    [[view_ bodyLabel] setText:[ad bodyText]];

    // ad call to action button
    [[view_ callToActionButton] setTitle:[ad callToAction]
                                forState:UIControlStateNormal];

    // ad social context label
    [[view_ socialContextLabel] setText:[ad socialContext]];

    // ad sponsored view
    [[view_ sponsorLabel] setText:[ad sponsoredTranslation]];

    // ad title view
    [[view_ titleLabel] setText:[ad headline]];

    // ad media view
    [[view_ mediaView] setDelegate:self];
    [[view_ iconView] setDelegate:self];

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
