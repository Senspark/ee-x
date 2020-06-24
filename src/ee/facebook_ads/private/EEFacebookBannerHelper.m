#import "ee/facebook_ads/private/EEFacebookBannerHelper.h"

#import <ee_x-Swift.h>

@implementation EEFacebookBannerHelper {
    NSMutableDictionary<NSNumber*, NSValue*>* sizes_;
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

@end
