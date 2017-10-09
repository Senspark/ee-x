//
//  EEFacebookBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import <FBAudienceNetwork/FBAdView.h>

#import "ee/core/internal/EEJsonUtils.h"
#import "ee/core/internal/EEMessageBridge.h"
#import "ee/core/internal/EEUtils.h"
#import "ee/facebook/internal/EEFacebookBannerAd.h"

@interface EEFacebookBannerAd () <FBAdViewDelegate> {
    FBAdView* adView_;
    FBAdSize adSize_;
    BOOL isAdLoaded_;
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

    isAdLoaded_ = NO;
    adSize_ = adSize;

    UIViewController* rootView = [EEUtils getCurrentRootViewController];
    FBAdView* adView =
        [[[FBAdView alloc] initWithPlacementID:adId
                                        adSize:adSize
                            rootViewController:rootView] autorelease];
    [adView setDelegate:self];
    [adView loadAd];
    [[rootView view] addSubview:adView];
    adView_ = adView;

    [self registerHandlers];
    return self;
}

- (void)dealloc {
    [self deregisterhandlers];
    [adView_ removeFromSuperview];
    adView_ = nil;
    [super dealloc];
}

- (NSString*)k__isLoaded {
    return [@"FacebookBannerAd_isLoaded_"
        stringByAppendingString:[adView_ placementID]];
}

- (NSString*)k__getPosition {
    return [@"FacebookBannerAd_getPosition_"
        stringByAppendingString:[adView_ placementID]];
}

- (NSString*)k__setPosition {
    return [@"FacebookBannerAd_setPosition_"
        stringByAppendingString:[adView_ placementID]];
}

- (NSString*)k__getSize {
    return [@"FacebookBannerAd_getSize_"
        stringByAppendingString:[adView_ placementID]];
}

- (NSString*)k__setSize {
    return [@"FacebookBannerAd_setSize_"
        stringByAppendingString:[adView_ placementID]];
}

- (NSString*)k__setVisible {
    return [@"FacebookBannerAd_setVisible_"
        stringByAppendingString:[adView_ placementID]];
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
    CGPoint position = [adView_ frame].origin;
    return CGPointMake(position.x * scale, position.y * scale);
}

- (void)setPosition:(CGPoint)position {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [adView_ frame];
    frame.origin = CGPointMake(position.x / scale, position.y);
    [adView_ setFrame:frame];
}

- (CGSize)getSize {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGSize size = [adView_ frame].size;
    return CGSizeMake(size.width * scale, size.height * scale);
}

- (void)setSize:(CGSize)size {
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGRect frame = [adView_ frame];
    frame.size = CGSizeMake(size.width / scale, size.height / scale);
    [adView_ setFrame:frame];
}

- (void)setVisible:(BOOL)visible {
    [adView_ setHidden:!visible];
}

- (void)adViewDidClick:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)adViewDidFinishHandlingClick:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)adViewDidLoad:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
    isAdLoaded_ = YES;
}

- (void)adView:(FBAdView*)adView didFailWithError:(NSError*)error {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, error);
}

- (void)adViewWillLogImpression:(FBAdView*)adView {
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

@end
