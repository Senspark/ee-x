//
//  EEFacebookNativeAd.hpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#import "ee/ads/internal/EEIAdView.h"

@interface EEFacebookNativeAd : NSObject <EEIAdView>

- (id _Nonnull)initWithAdId:(NSString* _Nonnull)adId
                     layout:(NSString* _Nonnull)layoutName;

- (void)destroy;

@end
