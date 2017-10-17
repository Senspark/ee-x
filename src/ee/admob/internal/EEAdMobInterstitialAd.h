//
//  EEAdMobInterstitialAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEInterstitialAdInterface.h"

@interface EEAdMobInterstitialAd : NSObject <EEInterstitialAdInterface>

- (id _Nonnull)initWithAdId:(NSString* _Nonnull)adId;

- (void)destroy;

@end
