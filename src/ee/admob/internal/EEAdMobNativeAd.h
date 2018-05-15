//
//  EEAdMobNativeAd.h
//  ee_x
//
//  Created by Zinge on 10/16/17.
//
//

#import "ee/ads/internal/EEIAdView.h"

@protocol EEIMessageBridge;

@interface EEAdMobNativeAd : NSObject <EEIAdView>

- (id _Nullable)initWithBridge:(id<EEIMessageBridge>)bridge
                          adId:(NSString* _Nonnull)adId
                         types:(NSArray<GADAdLoaderAdType>* _Nonnull)adTypes
                        layout:(NSString* _Nonnull)layoutName
                   testDevices:(NSArray<NSString*>* _Nullable)testDevices;

- (void)destroy;

@end
