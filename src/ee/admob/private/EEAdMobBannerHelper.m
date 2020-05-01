#import "ee/admob/private/EEAdMobBannerHelper.h"

#import <ee/core/internal/EEUtils.h>

@implementation EEAdMobBannerHelper {
    NSMutableDictionary<NSNumber*, NSValue*>* sizes_;
}

+ (CGSize)convertAdSizeToSize:(GADAdSize)adSize {
    CGSize size = CGSizeFromGADAdSize(adSize);
    return CGSizeMake([EEUtils convertDpToPixels:size.width],
                      [EEUtils convertDpToPixels:size.height]);
}

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }
    sizes_ = [[NSMutableDictionary alloc] init];
    for (int i = 0; i < 4; ++i) {
        GADAdSize adSize = [self getAdSize:i];
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

- (GADAdSize)getAdSize:(int)index {
    if (index == 0) {
        return kGADAdSizeBanner;
    }
    if (index == 1) {
        return kGADAdSizeLargeBanner;
    }
    if (index == 2) {
        if ([EEUtils isLandscape]) {
            return kGADAdSizeSmartBannerLandscape;
        }
        return kGADAdSizeSmartBannerPortrait;
    }
    if (index == 3) {
        return kGADAdSizeMediumRectangle;
    }
    NSAssert(NO, @"");
    return kGADAdSizeInvalid;
}

- (CGSize)getSize:(int)index {
    return [[sizes_ objectForKey:@(index)] CGSizeValue];
}

@end
