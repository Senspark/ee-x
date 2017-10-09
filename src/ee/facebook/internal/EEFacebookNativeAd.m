//
//  EEFacebookNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <FBAudienceNetwork/FBAudienceNetwork.h>

#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebook/EEFacebookNativeAdView.h"
#import "ee/facebook/internal/EEFacebookNativeAd.h"

@interface EEFacebookNativeAd () <FBNativeAdDelegate> {
    FBNativeAd* nativeAd_;
    EEFacebookNativeAdView* nativeAdView_;
    BOOL isAdLoaded_;
}

@end

@implementation EEFacebookNativeAd

- (id)initWithAdId:(NSString*)adId layout:(NSString*)layout {
    self = [super init];
    if (self == nil) {
        return self;
    }

    isAdLoaded_ = NO;

    FBNativeAd* nativeAd =
        [[[FBNativeAd alloc] initWithPlacementID:adId] autorelease];
    [nativeAd setDelegate:self];
    [nativeAd setMediaCachePolicy:FBNativeAdsCachePolicyAll];
    [nativeAd loadAd];
    nativeAd_ = [nativeAd retain];

    UIViewController* view = [EEUtils getCurrentRootViewController];
    EEFacebookNativeAdView* adView =
        [[[[NSBundle mainBundle] loadNibNamed:layout owner:nil options:nil]
            firstObject] autorelease];
    [adView setHidden:YES];
    [[adView adchoicesView] setCorner:UIRectCornerTopRight];
    [[view view] addSubview:adView];
    nativeAdView_ = adView;

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterhandlers];
    [nativeAd_ unregisterView];
    [nativeAd_ release];
    nativeAd_ = nil;
    [nativeAdView_ removeFromSuperview];
    [super dealloc];
}

- (NSString*)k__isLoaded {
    return [@"FacebookNativeAd_isLoaded_"
        stringByAppendingString:[nativeAd_ placementID]];
}

- (NSString*)k__getPosition {
    return [@"FacebookNativeAd_getPosition_"
        stringByAppendingString:[nativeAd_ placementID]];
}

- (NSString*)k__setPosition {
    return [@"FacebookNativeAd_setPosition_"
        stringByAppendingString:[nativeAd_ placementID]];
}

- (NSString*)k__getSize {
    return [@"FacebookNativeAd_getSize_"
        stringByAppendingString:[nativeAd_ placementID]];
}

- (NSString*)k__setSize {
    return [@"FacebookNativeAd_setSize_"
        stringByAppendingString:[nativeAd_ placementID]];
}

- (NSString*)k__setVisible {
    return [@"FacebookNativeAd_setVisible_"
        stringByAppendingString:[nativeAd_ placementID]];
}

- (void)registerHandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge registerHandler:[self k__isLoaded]
                   callback:^(NSString* message) {
                       return [self adLoaded] ? @"true" : @"false";
                   }];

    [bridge registerHandler:[self k__getPosition]
                   callback:^(NSString* message) {
                       CGPoint position = [self getPosition];
                       NSMutableDictionary* dict =
                           [NSMutableDictionary dictionary];
                       [dict setValue:@(position.x) forKey:@"x"];
                       [dict setValue:@(position.y) forKey:@"y"];
                       return [EEJsonUtils convertDictionaryToString:dict];
                   }];

    [bridge registerHandler:[self k__setPosition]
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       int x = [dict[@"x"] intValue];
                       int y = [dict[@"y"] intValue];
                       [self setPosition:CGPointMake(x, y)];
                       return @"";
                   }];

    [bridge registerHandler:[self k__getSize]
                   callback:^(NSString* message) {
                       CGSize size = [self getSize];
                       NSMutableDictionary* dict =
                           [NSMutableDictionary dictionary];
                       [dict setValue:@(size.width) forKey:@"width"];
                       [dict setValue:@(size.height) forKey:@"height"];
                       return [EEJsonUtils convertDictionaryToString:dict];
                   }];

    [bridge registerHandler:[self k__setSize]
                   callback:^(NSString* message) {
                       NSDictionary* dict =
                           [EEJsonUtils convertStringToDictionary:message];
                       int width = [dict[@"width"] intValue];
                       int height = [dict[@"height"] intValue];
                       [self setSize:CGSizeMake(width, height)];
                       return @"";
                   }];

    [bridge registerHandler:[self k__setVisible]
                   callback:^(NSString* message) {
                       [self setVisible:[message isEqualToString:@"true"]];
                       return @"";
                   }];
}

- (void)deregisterhandlers {
    EEMessageBridge* bridge = [EEMessageBridge getInstance];

    [bridge deregisterHandler:[self k__isLoaded]];
    [bridge deregisterHandler:[self k__getPosition]];
    [bridge deregisterHandler:[self k__setPosition]];
    [bridge deregisterHandler:[self k__getSize]];
    [bridge deregisterHandler:[self k__setSize]];
    [bridge deregisterHandler:[self k__setVisible]];
}

- (BOOL)isAdLoaded {
    return isAdLoaded_;
}

- (CGPoint)getPosition {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGPoint position = [nativeAdView_ frame].origin;
    return CGPointMake(position.x * scale, position.y * scale);
}

- (void)setPosition:(CGPoint)position {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [nativeAdView_ frame];
    frame.origin = CGPointMake(position.x / scale, position.y / scale);
    [nativeAdView_ setFrame:frame];
}

- (CGSize)getSize {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGSize size = [nativeAdView_ frame].size;
    return CGSizeMake(size.width / scale, size.height / scale);
}

- (void)setSize:(CGSize)size {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [nativeAdView_ frame];
    frame.size =
        CGSizeMake(frame.size.width / scale, frame.size.height / scale);
    [nativeAdView_ setFrame:frame];
}

- (void)setVisible:(BOOL)visible {
    [nativeAdView_ setHidden:!visible];
    if (visible) {
        for (UIView* subView in [nativeAdView_ subviews]) {
            [subView setNeedsDisplay];
        }
    }
}

- (void)nativeAdDidLoad:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSAssert(nativeAd == nativeAd_, @"");

    isAdLoaded_ = YES;

    UIViewController* view = [EEUtils getCurrentRootViewController];
    [nativeAd unregisterView];
    [nativeAd registerViewForInteraction:nativeAdView_
                      withViewController:view
                      withClickableViews:@[[nativeAdView_ callToActionButton]]];

    [[nativeAd coverImage] loadImageAsyncWithBlock:^(UIImage* image) {
        [[nativeAdView_ coverImage] setImage:image];
    }];

    [[nativeAd icon] loadImageAsyncWithBlock:^(UIImage* image) {
        [[nativeAdView_ iconImage] setImage:image];
    }];

    [[nativeAdView_ titleLabel] setText:[nativeAd title]];
    [[nativeAdView_ bodyLabel] setText:[nativeAd body]];
    [[nativeAdView_ socialContextLabel] setText:[nativeAd socialContext]];
    [[nativeAdView_ sponsorLabel] setText:@"Sponsored"];
    [[nativeAdView_ callToActionButton] setTitle:[nativeAd callToAction]
                                        forState:UIControlStateNormal];
    [[nativeAdView_ adchoicesView] setNativeAd:nativeAd];
}

- (void)nativeAdWillLogImpression:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)nativeAd:(FBNativeAd*)nativeAd didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, error);
}

- (void)nativeAdDidClick:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)nativeAdDidFinishHandlingClick:(FBNativeAd*)nativeAd {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
