//
//  EEFacebookBannerAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import "ee/ads/internal/EEIAdView.h"

@class EEMessageBridge;

@interface EEFacebookBannerAd : NSObject <EEIAdView>

+ (FBAdSize)adSizeFor:(int)index;

- (id _Nonnull)initWithBridge:(EEMessageBridge* _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(FBAdSize)adSize;

- (void)destroy;

@end
