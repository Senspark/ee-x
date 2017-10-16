//
//  EEAdMobNativeAd.m
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#import <GoogleMobileAds/GoogleMobileAds.h>

#import "ee/admob/internal/EEAdMobNativeAd.h"
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
}

@end

@implementation EEAdMobNativeAd

- (id _Nullable)initWithId:(NSString* _Nonnull)adId
                     types:(NSArray<GADAdLoaderAdType>* _Nonnull)adTypes
                    layout:(NSString* _Nonnull)layoutName {
    self = [super init];
    if (self == nil) {
        return self;
    }
    isAdLoaded_ = NO;
    adId_ = [adId copy];
    adTypes_ = [adTypes copy];
    layoutName_ = [layoutName copy];
    [self createInternalAd];
    [self createView];
    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterHandlers];
    [self destroyInternalAd];
    [self destroyView];
    [adId_ release];
    adId_ = nil;
    [adTypes_ release];
    adTypes_ = nil;
    [layoutName_ release];
    layoutName_ = nil;
    [super dealloc];
}

- (void)registerHandlers {
    //
}

- (void)deregisterHandlers {
    //
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
    GADRequest* request = [GADRequest request];
    [adLoader_ loadRequest:request];
}

- (CGPoint)getPosition {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGPoint position = [nativeAdPlaceholder_ frame].origin;
    return CGPointMake(position.x * scale, position.y * scale);
}

- (void)setPosition:(CGPoint)position {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [nativeAdPlaceholder_ frame];
    frame.origin = CGPointMake(position.x / scale, position.y / scale);
    [nativeAdPlaceholder_ setFrame:frame];
}

- (CGSize)getSize {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGSize size = [nativeAdPlaceholder_ frame].size;
    return CGSizeMake(size.width * scale, size.height * scale);
}

- (void)setSize:(CGSize)size {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [nativeAdPlaceholder_ frame];
    frame.size = CGSizeMake(size.width / scale, size.height / scale);
    [nativeAdPlaceholder_ setFrame:frame];
}

- (void)setVisible:(BOOL)visible {
    [nativeAdPlaceholder_ setHidden:!visible];
}

- (void)adLoader:(GADAdLoader*)adLoader
    didFailToReceiveAdWithError:(GADRequestError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [error description]);
    NSAssert(adLoader_ == adLoader, @"");
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
        [[[[NSBundle mainBundle] loadNibNamed:layoutName_ owner:nil options:nil]
            firstObject] autorelease];
    [self setAdView:adView];

    // Associate the app install ad view with the app install ad object. This is
    // required to make the ad clickable.
    [adView setNativeAppInstallAd:nativeAppInstallAd];

    // Populate the app install ad view with the app install ad assets.
    // Some assets are guaranteed to be present in every app install ad.
    UILabel* headLineView = (UILabel*)[adView headlineView];
    UIImageView* iconView = (UIImageView*)[adView iconView];
    UILabel* bodyView = (UILabel*)[adView bodyView];
    UIButton* callToActionView = (UIButton*)[adView callToActionView];

    [headLineView setText:[nativeAppInstallAd headline]];
    [iconView setImage:[[nativeAppInstallAd icon] image]];
    [bodyView setText:[nativeAppInstallAd body]];
    [callToActionView setTitle:[nativeAppInstallAd callToAction]
                      forState:UIControlStateNormal];

    // Some app install ads will include a video asset, while others do not.
    // Apps can use the GADVideoController's hasVideoContent property to
    // determine if one is present, and adjust their UI accordingly.

    // The UI for this controller constrains the image view's height to match
    // the media view's height, so by changing the one here, the height of both
    // views are being adjusted.
    if ([[nativeAppInstallAd videoController] hasVideoContent]) {
        [[adView mediaView] setHidden:NO];

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
    }

    // These assets are not guaranteed to be present, and should be checked
    // first.
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

    NSString* price = [nativeAppInstallAd price];
    UILabel* priceView = (UILabel*)[adView priceView];
    if (price != nil) {
        [priceView setText:price];
        [priceView setHidden:NO];
    } else {
        [priceView setHidden:YES];
    }

    // In order for the SDK to process touch events properly, user interaction
    // should be disabled.
    [[adView callToActionView] setUserInteractionEnabled:NO];
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

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    NSDictionary* viewDictionary =
        NSDictionaryOfVariableBindings(nativeAdView_);
    [[rootView view]
        addConstraints:[NSLayoutConstraint
                           constraintsWithVisualFormat:@"H:|[nativeAdView_]|"
                                               options:0
                                               metrics:nil
                                                 views:viewDictionary]];
    [[rootView view]
        addConstraints:[NSLayoutConstraint
                           constraintsWithVisualFormat:@"V:|[nativeAdView_]|"
                                               options:0
                                               metrics:nil
                                                 views:viewDictionary]];
}

@end
