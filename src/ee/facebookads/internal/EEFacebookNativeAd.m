//
//  EEFacebookNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import "ee/facebookads/EEFacebookNativeAdView.h"

#ifdef EE_X_USE_IRON_SOURCE_MEDIATION
#import <ISFacebookAdapter/FBAdChoicesView.h>
#import <ISFacebookAdapter/FBMediaView.h>
#import <ISFacebookAdapter/FBNativeAd.h>
#else // EE_X_USE_IRON_SOURCE_MEDIATION
#import <FBAudienceNetwork/FBAdChoicesView.h>
#import <FBAudienceNetwork/FBMediaView.h>
#import <FBAudienceNetwork/FBNativeAd.h>
#endif // EE_X_USE_IRON_SOURCE_MEDIATION

#import "ee/ads/internal/EEAdViewHelper.h"
#import "ee/core/EEIMessageBridge.h"
#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebookads/internal/EEFacebookNativeAd.h"

@interface EEFacebookNativeAd () <FBNativeAdDelegate, FBMediaViewDelegate> {
    id<EEIMessageBridge> bridge_;

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

    /// Identifiers
    NSDictionary* identifiers_;
}

@end

@implementation EEFacebookNativeAd

static NSString* const k__tag = @"FacebookNativeAd";

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
    isAdLoaded_ = NO;
    adId_ = [adId copy];
    layoutName_ = [layoutName copy];
    nativeAd_ = nil;
    nativeAdView_ = nil;
    identifiers_ = [identifiers copy];
    helper_ = [[EEAdViewHelper alloc] initWithBridge:bridge_
                                              prefix:k__tag
                                                adId:adId_];

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
    [identifiers_ release];
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

- (NSString* _Nonnull)k__onClicked {
    return [NSString stringWithFormat:@"%@_onClicked_%@", k__tag, adId_];
}

- (void)registerHandlers {
    [helper_ registerHandlers:self];
    [bridge_ registerHandler:[self k__createInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self createInternalAd]];
                    }];
    [bridge_ registerHandler:[self k__destroyInternalAd]
                    callback:^(NSString* message) {
                        return [EEUtils toString:[self destroyInternalAd]];
                    }];
}

- (void)deregisterhandlers {
    [helper_ deregisterHandlers];
    [bridge_ deregisterHandler:[self k__createInternalAd]];
    [bridge_ deregisterHandler:[self k__destroyInternalAd]];
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
    [adView adchoicesView].corner = UIRectCornerTopRight;

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

    if ([nativeAdView_ callToActionButton]) {
        [nativeAd
            registerViewForInteraction:nativeAdView_
                             mediaView:[nativeAdView_ mediaView]
                         iconImageView:[nativeAdView_ iconImage]
                        viewController:rootView
                        clickableViews:@[[nativeAdView_ callToActionButton]]];
    }

    // adchoices view
    [[nativeAdView_ adchoicesView] setNativeAd:nativeAd];

    // ad body view
    [[nativeAdView_ bodyLabel] setText:[nativeAd bodyText]];

    // ad call to action button
    [[nativeAdView_ callToActionButton] setTitle:[nativeAd callToAction]
                                        forState:UIControlStateNormal];

    //disable click on mediaView if it's not contain FBMediaViewVideoRenderer
    BOOL isContainVideo = NO;
    NSArray<UIView*> *subviews = [nativeAdView_ mediaView].subviews;
    for (UIView* iter in subviews) {
        if ([iter isKindOfClass:[FBMediaViewVideoRenderer class]]) {
            isContainVideo = YES;
            break;
        }
    }
    
    if (!isContainVideo) {
        [nativeAdView_ mediaView].userInteractionEnabled = NO;
    } else {
        [nativeAdView_ mediaView].userInteractionEnabled = YES;
    }
    
    // ad social context label
    [[nativeAdView_ socialContextLabel] setText:[nativeAd socialContext]];

    // ad sponsored view
    [[nativeAdView_ sponsorLabel] setText:@"Sponsored"];

    // ad title view
    [[nativeAdView_ titleLabel] setText:[nativeAd headline]];

    // ad media view
    [[nativeAdView_ mediaView] setDelegate:self];

    isAdLoaded_ = YES;
    [bridge_ callCpp:[self k__onLoaded]];
}

- (void)nativeAdWillLogImpression:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)nativeAd:(FBNativeAd*)nativeAd didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    [bridge_ callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)nativeAdDidClick:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:[self k__onClicked]];
}

- (void)nativeAdDidFinishHandlingClick:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)mediaViewDidLoad:(FBMediaView*)mediaView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    // hidden cover image when media did load.
    [[nativeAdView_ coverImage] setHidden:[identifiers_ objectForKey:k__media]];
}
@end
