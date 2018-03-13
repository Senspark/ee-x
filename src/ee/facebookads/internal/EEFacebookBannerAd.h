//
//  EEFacebookBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import "ee/ads/internal/EEAdViewInterface.h"

@interface EEFacebookBannerAd : NSObject <EEAdViewInterface>

+ (FBAdSize)adSizeFor:(int)index;

- (id _Nonnull)initWithAdId:(NSString* _Nonnull)adId size:(FBAdSize)adSize;

- (void)destroy;

@end
