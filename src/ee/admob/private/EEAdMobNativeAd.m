//
//  EEAdMobNativeAd.m
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#import "ee/admob/private/EEAdMobNativeAd.h"

#import <GoogleMobileAds/GADUnifiedNativeAd.h>

#import <ee_x-Swift.h>

#import <ee/ads/internal/EEAdViewHelper.h>
#import <ee/ads/internal/EEMessageHelper.h>
#import <ee/ads/internal/EEViewHelper.h>

@interface EEAdMobNativeAd () <GADUnifiedNativeAdLoaderDelegate,
                               GADUnifiedNativeAdDelegate>
@end

@implementation EEAdMobNativeAd {
    id<EEIMessageBridge> bridge_;
    NSString* adId_;
    NSArray<GADAdLoaderAdType>* adTypes_;
    NSString* layoutName_;
    EEMessageHelper* messageHelper_;
    EEAdViewHelper* helper_;
    EEViewHelper* viewHelper_;
    BOOL isLoaded_;
    GADAdLoader* ad_;
    UIView* view_;
    GADUnifiedNativeAdView* nativeAd_;
}

- (id _Nullable)initWithBridge:(id<EEIMessageBridge>)bridge
                          adId:(NSString* _Nonnull)adId
                         types:(NSArray<GADAdLoaderAdType>* _Nonnull)adTypes
                        layout:(NSString* _Nonnull)layoutName {
    NSAssert([EEUtils isMainThread], @"");
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    adId_ = [adId copy];
    adTypes_ = [adTypes copy];
    layoutName_ = [layoutName copy];
    messageHelper_ =
        [[EEMessageHelper alloc] initWithPrefix:@"AdMobNativeAd" adId:adId];
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
    [self deregisterHandlers];
    [self destroyView];
    [self destroyInternalAd];

    [adId_ release];
    adId_ = nil;
    [adTypes_ release];
    adTypes_ = nil;
    [layoutName_ release];
    layoutName_ = nil;
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

- (void)deregisterHandlers {
    [helper_ deregisterHandlers];
}

- (BOOL)createInternalAd {
    NSAssert([EEUtils isMainThread], @"");
    if (ad_ != nil) {
        return NO;
    }
    isLoaded_ = NO;

    GADVideoOptions* options = [[[GADVideoOptions alloc] init] autorelease];
    options.startMuted = YES;

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    ad_ = [[GADAdLoader alloc] initWithAdUnitID:adId_
                             rootViewController:rootView
                                        adTypes:adTypes_
                                        options:@[options]];
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
    [ad_ release];
    ad_ = nil;
    return YES;
}

- (void)createView {
    NSAssert([EEUtils isMainThread], @"");
    NSAssert(view_ == nil, @"");
    UIView* view = [[[UIView alloc] init] autorelease];
    [view setHidden:YES];

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [[rootView view] addSubview:view];

    view_ = [view retain];
    viewHelper_ = [[EEViewHelper alloc] initWithView:view_];

    NSArray* nibObjects =
        [[NSBundle mainBundle] loadNibNamed:layoutName_ owner:nil options:nil];
    [self setAdView:[nibObjects firstObject]];
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
    isLoaded_ = NO;
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
    if (visible) {
        for (UIView* subView in [nativeAd_ subviews]) {
            [subView setNeedsDisplay];
        }
    }
}

#pragma mark - GADAdLoaderDelegate implementation

- (void)adLoader:(GADAdLoader*)adLoader
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(ad_ == adLoader, @"");
    [bridge_ callCpp:[messageHelper_ onFailedToLoad] //
                    :[error description]];
}

- (void)adLoaderDidFinishLoading:(GADAdLoader*)adLoader {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(ad_ == adLoader, @"");
}

/// Gets an image representing the number of stars. Returns nil if rating is
/// less than 3.5 stars.
- (UIImage*)imageForStars:(NSDecimalNumber*)numberOfStars {
    double starRating = [numberOfStars doubleValue];
    if (starRating >= 5) {
        return [UIImage imageNamed:@"stars_5"];
    } else if (starRating >= 4.5) {
        return [UIImage imageNamed:@"stars_4_5"];
    } else if (starRating >= 4) {
        return [UIImage imageNamed:@"stars_4"];
    } else if (starRating >= 3.5) {
        return [UIImage imageNamed:@"stars_3_5"];
    } else {
        return nil;
    }
}

- (void)setAdView:(GADUnifiedNativeAdView* _Nonnull)view {
    if (nativeAd_ != nil) {
        // Remove previous ad view.
        [nativeAd_ removeFromSuperview];
    }
    nativeAd_ = view;

    // Add new ad view and set constraints to fill its container.
    [view_ addSubview:view];

    [view setTranslatesAutoresizingMaskIntoConstraints:NO];

    NSDictionary* viewDictionary = NSDictionaryOfVariableBindings(nativeAd_);
    [view_ addConstraints:[NSLayoutConstraint
                              constraintsWithVisualFormat:@"H:|[nativeAd_]|"
                                                  options:0
                                                  metrics:nil
                                                    views:viewDictionary]];
    [view_ addConstraints:[NSLayoutConstraint
                              constraintsWithVisualFormat:@"V:|[nativeAd_]|"
                                                  options:0
                                                  metrics:nil
                                                    views:viewDictionary]];
}

#pragma mark - GADUnifiedNativeAdDelegate

// The native ad was shown.
- (void)nativeAdDidRecordImpression:(GADUnifiedNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

// The native ad was clicked on.
- (void)nativeAdDidRecordClick:(GADUnifiedNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

// The native ad will present a full screen view.
- (void)nativeAdWillPresentScreen:(GADUnifiedNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

// The native ad will dismiss a full screen view.
- (void)nativeAdWillDismissScreen:(GADUnifiedNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

// The native ad did dismiss a full screen view.
- (void)nativeAdDidDismissScreen:(GADUnifiedNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

// The native ad will cause the application to become inactive and open a new
// application.
- (void)nativeAdWillLeaveApplication:(GADUnifiedNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [bridge_ callCpp:[messageHelper_ onClicked]];
}

#pragma mark - GADUnifiedNativeAdLoaderDelegate implementation

- (void)adLoader:(nonnull GADAdLoader*)adLoader
    didReceiveUnifiedNativeAd:(nonnull GADUnifiedNativeAd*)nativeAd {
    GADUnifiedNativeAdView* nativeAdView = nativeAd_;

    nativeAdView.nativeAd = nativeAd;

    // Set ourselves as the ad delegate to be notified of native ad events.
    nativeAd.delegate = self;

    // Populate the native ad view with the native ad assets.
    // The headline and mediaContent are guaranteed to be present in every
    // native ad.
    ((UILabel*)nativeAdView.headlineView).text = nativeAd.headline;
    nativeAdView.mediaView.mediaContent = nativeAd.mediaContent;

    // These assets are not guaranteed to be present. Check that they are before
    // showing or hiding them.
    ((UILabel*)nativeAdView.bodyView).text = nativeAd.body;
    nativeAdView.bodyView.hidden = nativeAd.body ? NO : YES;

    [((UIButton*)nativeAdView.callToActionView) setTitle:nativeAd.callToAction
                                                forState:UIControlStateNormal];
    nativeAdView.callToActionView.hidden = nativeAd.callToAction ? NO : YES;

    ((UIImageView*)nativeAdView.iconView).image = nativeAd.icon.image;
    nativeAdView.iconView.hidden = nativeAd.icon ? NO : YES;

    ((UIImageView*)nativeAdView.starRatingView).image =
        [self imageForStars:nativeAd.starRating];
    nativeAdView.starRatingView.hidden = nativeAd.starRating ? NO : YES;

    ((UILabel*)nativeAdView.storeView).text = nativeAd.store;
    nativeAdView.storeView.hidden = nativeAd.store ? NO : YES;

    ((UILabel*)nativeAdView.priceView).text = nativeAd.price;
    nativeAdView.priceView.hidden = nativeAd.price ? NO : YES;

    ((UILabel*)nativeAdView.advertiserView).text = nativeAd.advertiser;
    nativeAdView.advertiserView.hidden = nativeAd.advertiser ? NO : YES;

    // In order for the SDK to process touch events properly, user interaction
    // should be disabled.
    nativeAdView.callToActionView.userInteractionEnabled = NO;

    isLoaded_ = YES;
    [bridge_ callCpp:[messageHelper_ onLoaded]];
}

@end
