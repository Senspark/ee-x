//
//  EEAdMobBannerAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEIAdView.h"

@class EEMessageBridge;

@interface EEAdMobBannerAd : NSObject <EEIAdView>

+ (GADAdSize)adSizeFor:(int)index;

- (id _Nonnull)initWithBridge:(EEMessageBridge* _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(GADAdSize)adSize
                  testDevices:(NSArray<NSString*>* _Nullable)testDevices;

- (void)destroy;

@end
