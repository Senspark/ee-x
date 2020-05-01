#import "ee/facebook_ads/private/EEFacebookBannerHelper.h"

#import <ee/core/internal/EEUtils.h>

@implementation EEFacebookBannerHelper {
    NSMutableDictionary<NSNumber*, NSValue*>* sizes_;
}

+ (CGFloat)getWidthInDp:(FBAdSize)adSize {
    int width = (int)roundf(adSize.size.width);
    switch (width) {
    case 0:  // Interstitial ad.
    case -1: // Normal ads.
        return [[UIScreen mainScreen] bounds].size.width;
    default: // Deprecated ads.
        return width;
    }
}

+ (CGFloat)getHeightInDp:(FBAdSize)adSize {
    int height = (int)roundf(adSize.size.height);
    switch (height) {
    case 0: // Interstitial ad.
        return [[UIScreen mainScreen] bounds].size.height;
    default: // Normal ads.
        return height;
    }
}

+ (CGSize)convertAdSizeToSize:(FBAdSize)adSize {
    CGSize size =
        CGSizeMake([self getWidthInDp:adSize], [self getHeightInDp:adSize]);
    return CGSizeMake([EEUtils convertDpToPixels:size.width],
                      [EEUtils convertDpToPixels:size.height]);
}

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    sizes_ = [[NSMutableDictionary alloc] init];
    for (int i = 0; i < 5; ++i) {
        FBAdSize adSize = [self getAdSize:i];
        CGSize size = [[self class] convertAdSizeToSize:adSize];
        [sizes_ setObject:[NSValue valueWithCGSize:size] forKey:@(i)];
    }
    return self;
}

- (void)dealloc {
    [sizes_ release];
    sizes_ = nil;
    [super dealloc];
}

- (FBAdSize)getAdSize:(int)index {
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
    if (index == 4) {
        return kFBAdSize320x50;
    }
    NSAssert(NO, @"");
    return kFBAdSize320x50;
}

- (CGSize)getSize:(int)index {
    return [[sizes_ objectForKey:@(index)] CGSizeValue];
}

@end
