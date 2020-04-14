//
//  EEAdMobNativeAd.m
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#import "ee/admob/private/EEAdMobNativeAd.h"

#import <GoogleMobileAds/GoogleMobileAds.h>

#import <ee/ads/internal/EEAdViewHelper.h>
#import <ee/ads/internal/EEViewHelper.h>
#import <ee/core/internal/EEIMessageBridge.h>
#import <ee/core/internal/EEUtils.h>

@interface EEAdMobNativeAd () <GADUnifiedNativeAdLoaderDelegate,
                               GADUnifiedNativeAdDelegate> {
    id<EEIMessageBridge> bridge_;
    GADAdLoader* adLoader_;
    NSString* adId_;
    NSArray<GADAdLoaderAdType>* adTypes_;
    UIView* nativeAdPlaceholder_;
    GADUnifiedNativeAdView* nativeAdView_;
    NSString* layoutName_;
    BOOL isAdLoaded_;
    EEAdViewHelper* helper_;
    EEViewHelper* viewHelper_;
}

@end

@implementation EEAdMobNativeAd

- (id _Nullable)initWithBridge:(id<EEIMessageBridge>)bridge
                          adId:(NSString* _Nonnull)adId
                         types:(NSArray<GADAdLoaderAdType>* _Nonnull)adTypes
                        layout:(NSString* _Nonnull)layoutName {
    self = [super init];
    if (self == nil) {
        return self;
    }
    bridge_ = bridge;
    isAdLoaded_ = NO;
    adId_ = [adId copy];
    adTypes_ = [adTypes copy];
    layoutName_ = [layoutName copy];
    helper_ = [[EEAdViewHelper alloc] initWithBridge:bridge_
                                                view:self
                                              prefix:@"AdMobNativeAd"
                                                adId:adId];
    viewHelper_ = nil;

    [self createInternalAd];
    [self createView];
    [self registerHandlers];
    return self;
}

- (void)destroy {
    [self deregisterHandlers];
    [self destroyInternalAd];
    [self destroyView];
}

- (void)dealloc {
    [helper_ release];
    helper_ = nil;
    [adId_ release];
    adId_ = nil;
    [adTypes_ release];
    adTypes_ = nil;
    [layoutName_ release];
    layoutName_ = nil;
    [super dealloc];
}

- (NSString*)k__onLoaded {
    return [@"AdMobNativeAd_onLoaded_" stringByAppendingString:adId_];
}

- (NSString*)k__onFailedToLoad {
    return [@"AdMobNativeAd_onFailedToLoad_" stringByAppendingString:adId_];
}

- (NSString*)k__onClicked {
    return [@"AdMobNativeAd_onClicked_" stringByAppendingString:adId_];
}

- (void)registerHandlers {
    [helper_ registerHandlers];
}

- (void)deregisterHandlers {
    [helper_ deregisterHandlers];
}

- (BOOL)createInternalAd {
    if (adLoader_ != nil) {
        return NO;
    }
    isAdLoaded_ = NO;
    UIViewController* rootView = [EEUtils getCurrentRootViewController];

    GADVideoOptions* videoOptions = [[GADVideoOptions alloc] init];
    videoOptions.startMuted = YES;

    GADAdLoader* loader =
        [[[GADAdLoader alloc] initWithAdUnitID:adId_
                            rootViewController:rootView
                                       adTypes:adTypes_
                                       options:@[videoOptions]] autorelease];
    [loader setDelegate:self];
    adLoader_ = [loader retain];
    return YES;
}

- (BOOL)destroyInternalAd {
    if (adLoader_ == nil) {
        return NO;
    }
    isAdLoaded_ = NO;
    [adLoader_ release];
    adLoader_ = nil;
    return YES;
}

- (void)createView {
    NSAssert(nativeAdPlaceholder_ == nil, @"");
    UIView* view = [[[UIView alloc] init] autorelease];
    [view setHidden:YES];

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    [[rootView view] addSubview:view];

    nativeAdPlaceholder_ = [view retain];
    viewHelper_ = [[EEViewHelper alloc] initWithView:nativeAdPlaceholder_];

    NSArray* nibObjects =
        [[NSBundle mainBundle] loadNibNamed:layoutName_ owner:nil options:nil];
    [self setAdView:[nibObjects firstObject]];
}

- (void)destroyView {
    NSAssert(nativeAdPlaceholder_ != nil, @"");
    [viewHelper_ release];
    viewHelper_ = nil;
    [nativeAdPlaceholder_ removeFromSuperview];
    [nativeAdPlaceholder_ release];
    nativeAdPlaceholder_ = nil;
}

- (BOOL)isLoaded {
    if (adLoader_ == nil) {
        return NO;
    }
    return isAdLoaded_;
}

- (void)load {
    if (adLoader_ == nil) {
        return;
    }
    isAdLoaded_ = NO;
    GADRequest* request = [GADRequest request];
    [adLoader_ loadRequest:request];
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

#pragma mark GADAdLoaderDelegate implementation

- (void)adLoader:(GADAdLoader*)adLoader
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(adLoader_ == adLoader, @"");
    [bridge_ callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)adLoaderDidFinishLoading:(GADAdLoader*)adLoader {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(adLoader_ == adLoader, @"");
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
    if (nativeAdView_ != nil) {
        // Remove previous ad view.
        [nativeAdView_ removeFromSuperview];
    }
    nativeAdView_ = view;

    // Add new ad view and set constraints to fill its container.
    [nativeAdPlaceholder_ addSubview:view];

    [view setTranslatesAutoresizingMaskIntoConstraints:NO];

    NSDictionary* viewDictionary =
        NSDictionaryOfVariableBindings(nativeAdView_);
    [nativeAdPlaceholder_
        addConstraints:[NSLayoutConstraint
                           constraintsWithVisualFormat:@"H:|[nativeAdView_]|"
                                               options:0
                                               metrics:nil
                                                 views:viewDictionary]];
    [nativeAdPlaceholder_
        addConstraints:[NSLayoutConstraint
                           constraintsWithVisualFormat:@"V:|[nativeAdView_]|"
                                               options:0
                                               metrics:nil
                                                 views:viewDictionary]];
}

#pragma mark GADUnifiedNativeAdDelegate

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
    [bridge_ callCpp:[self k__onClicked]];
}

#pragma mark GADUnifiedNativeAdLoaderDelegate implementation
- (void)adLoader:(nonnull GADAdLoader*)adLoader
    didReceiveUnifiedNativeAd:(nonnull GADUnifiedNativeAd*)nativeAd {
    GADUnifiedNativeAdView* nativeAdView = nativeAdView_;

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

    isAdLoaded_ = YES;
    [bridge_ callCpp:[self k__onLoaded]];
}

@end
