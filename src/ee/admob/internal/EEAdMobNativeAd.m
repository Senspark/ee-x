//
//  EEAdMobNativeAd.m
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#import <GoogleMobileAds/GoogleMobileAds.h>

#import "ee/admob/internal/EEAdMobNativeAd.h"
#import "ee/ads/internal/EEAdViewHelper.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"

@interface EEAdMobNativeAd () <GADAdLoaderDelegate,
                               GADNativeAppInstallAdLoaderDelegate> {
    GADAdLoader* adLoader_;
    NSString* adId_;
    NSArray<GADAdLoaderAdType>* adTypes_;
    UIView* nativeAdPlaceholder_;
    UIView* nativeAdView_;
    NSString* layoutName_;
    BOOL isAdLoaded_;
    NSArray<NSString*>* testDevices_;
    EEAdViewHelper* helper_;
}

@end

@implementation EEAdMobNativeAd

- (id _Nullable)initWithId:(NSString* _Nonnull)adId
                     types:(NSArray<GADAdLoaderAdType>* _Nonnull)adTypes
                    layout:(NSString* _Nonnull)layoutName
               testDevices:(NSArray<NSString*>* _Nullable)testDevices {
    self = [super init];
    if (self == nil) {
        return self;
    }
    isAdLoaded_ = NO;
    adId_ = [adId copy];
    adTypes_ = [adTypes copy];
    layoutName_ = [layoutName copy];
    testDevices_ = [testDevices retain];
    helper_ =
        [[EEAdViewHelper alloc] initWithPrefix:@"AdMobNativeAd" adId:adId];
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
    [testDevices_ release];
    testDevices_ = nil;
    [super dealloc];
}

- (NSString*)k__onLoaded {
    return [@"AdMobNativeAd_onLoaded_" stringByAppendingString:adId_];
}

- (NSString*)k__onFailedToLoad {
    return [@"AdMobNativeAd_onFailedToLoad_" stringByAppendingString:adId_];
}

- (void)registerHandlers {
    [helper_ registerHandlers:self];
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
    GADAdLoader* loader =
        [[[GADAdLoader alloc] initWithAdUnitID:adId_
                            rootViewController:rootView
                                       adTypes:adTypes_
                                       options:nil] autorelease];
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
}

- (void)destroyView {
    NSAssert(nativeAdPlaceholder_ != nil, @"");
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
    [request setTestDevices:testDevices_];
    [adLoader_ loadRequest:request];
}

- (CGPoint)getPosition {
    return [EEAdViewHelper getPosition:nativeAdPlaceholder_];
}

- (void)setPosition:(CGPoint)position {
    [EEAdViewHelper setPosition:position for:nativeAdPlaceholder_];
}

- (CGSize)getSize {
    return [EEAdViewHelper getSize:nativeAdPlaceholder_];
}

- (void)setSize:(CGSize)size {
    [EEAdViewHelper setSize:size for:nativeAdPlaceholder_];
}

- (void)setVisible:(BOOL)visible {
    [EEAdViewHelper setVisible:visible for:nativeAdPlaceholder_];
    if (visible) {
        for (UIView* subView in [nativeAdView_ subviews]) {
            [subView setNeedsDisplay];
        }
    }
}

- (void)adLoader:(GADAdLoader*)adLoader
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(adLoader_ == adLoader, @"");

    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onFailedToLoad] message:[error description]];
}

- (void)adLoaderDidFinishLoading:(GADAdLoader*)adLoader {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(adLoader_ == adLoader, @"");
}

- (void)adLoader:(GADAdLoader*)adLoader
    didReceiveNativeAppInstallAd:(GADNativeAppInstallAd*)nativeAppInstallAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(adLoader_ == adLoader, @"");

    GADNativeAppInstallAdView* adView =
        [[[NSBundle mainBundle] loadNibNamed:layoutName_ owner:nil options:nil]
            firstObject];
    [self setAdView:adView];

    // Associate the app install ad view with the app install ad object. This is
    // required to make the ad clickable.
    [adView setNativeAppInstallAd:nativeAppInstallAd];

    // Populate the app install ad view with the app install ad assets.
    // Some assets are guaranteed to be present in every app install ad.
    UILabel* bodyView = (UILabel*)[adView bodyView];
    UIButton* callToActionView = (UIButton*)[adView callToActionView];
    UILabel* headLineView = (UILabel*)[adView headlineView];
    UIImageView* iconView = (UIImageView*)[adView iconView];

    [bodyView setText:[nativeAppInstallAd body]];
    [callToActionView setTitle:[nativeAppInstallAd callToAction]
                      forState:UIControlStateNormal];
    [headLineView setText:[nativeAppInstallAd headline]];
    [iconView setImage:[[nativeAppInstallAd icon] image]];

    // Some app install ads will include a video asset, while others do not.
    // Apps can use the GADVideoController's hasVideoContent property to
    // determine if one is present, and adjust their UI accordingly.

    // The UI for this controller constrains the image view's height to match
    // the media view's height, so by changing the one here, the height of both
    // views are being adjusted.
    UIImageView* imageView = (UIImageView*)[adView imageView];
    if ([[nativeAppInstallAd videoController] hasVideoContent]) {
        [[adView mediaView] setHidden:NO];
        [imageView setHidden:YES];

        // This app uses a fixed width for the GADMediaView and changes its
        // height to match the aspect ratio of the video it displays.
        NSLayoutConstraint* constraint = [NSLayoutConstraint
            constraintWithItem:[adView mediaView]
                     attribute:NSLayoutAttributeHeight
                     relatedBy:NSLayoutRelationEqual
                        toItem:[adView mediaView]
                     attribute:NSLayoutAttributeWidth
                    multiplier:(1 / [[nativeAppInstallAd videoController]
                                        aspectRatio])
                      constant:0];
        [constraint setActive:YES];
    } else {
        [[adView mediaView] setHidden:YES];
        [imageView setHidden:NO];

        GADNativeAdImage* firstImage =
            [[nativeAppInstallAd images] firstObject];
        [imageView setImage:[firstImage image]];
    }

    // These assets are not guaranteed to be present, and should be checked
    // first.
    NSString* price = [nativeAppInstallAd price];
    UILabel* priceView = (UILabel*)[adView priceView];
    if (price != nil) {
        [priceView setText:price];
        [priceView setHidden:NO];
    } else {
        [priceView setHidden:YES];
    }

    NSDecimalNumber* starRating = [nativeAppInstallAd starRating];
    UIImageView* starRatingView = (UIImageView*)[adView starRatingView];
    if (starRating != nil) {
        [starRatingView setImage:[self imageForStars:starRating]];
        [starRatingView setHidden:NO];
    } else {
        [starRatingView setHidden:YES];
    }

    NSString* store = [nativeAppInstallAd store];
    UILabel* storeView = (UILabel*)[adView storeView];
    if (store != nil) {
        [storeView setText:store];
        [storeView setHidden:NO];
    } else {
        [storeView setHidden:YES];
    }

    // In order for the SDK to process touch events properly, user interaction
    // should be disabled.
    [[adView callToActionView] setUserInteractionEnabled:NO];

    isAdLoaded_ = YES;
    EEMessageBridge* bridge = [EEMessageBridge getInstance];
    [bridge callCpp:[self k__onLoaded]];
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

- (void)setAdView:(UIView* _Nonnull)view {
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

@end
