//
//  EEAdMobBannerAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEAdViewInterface.h"

@interface EEAdMobBannerAd : NSObject <EEAdViewInterface>

+ (GADAdSize)adSizeFor:(int)index;

- (id _Nonnull)initWithAdId:(NSString* _Nonnull)adId size:(GADAdSize)adSize;

@end
