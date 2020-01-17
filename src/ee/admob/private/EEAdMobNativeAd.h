//
//  EEAdMobNativeAd.h
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#import <GoogleMobileAds/GADAdLoaderAdTypes.h>

#import <ee/ads/internal/EEIAdView.h>

@protocol EEIMessageBridge;

@interface EEAdMobNativeAd : NSObject <EEIAdView>

- (id _Nullable)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                          adId:(NSString* _Nonnull)adId
                         types:(NSArray<GADAdLoaderAdType>* _Nonnull)adTypes
                        layout:(NSString* _Nonnull)layoutName;

- (void)destroy;

@end
