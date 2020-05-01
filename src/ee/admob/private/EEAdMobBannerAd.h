//
//  EEAdMobBannerAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <GoogleMobileAds/GADAdSize.h>

#import <ee/ads/internal/EEIAdView.h>

@protocol EEIMessageBridge;

@interface EEAdMobBannerAd : NSObject <EEIAdView>

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(GADAdSize)adSize;

- (void)destroy;

@end
