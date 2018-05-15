//
//  EEAdMobBannerAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEIAdView.h"

@protocol EEIMessageBridge;

@interface EEAdMobBannerAd : NSObject <EEIAdView>

+ (GADAdSize)adSizeFor:(int)index;

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId
                         size:(GADAdSize)adSize
                  testDevices:(NSArray<NSString*>* _Nullable)testDevices;

- (void)destroy;

@end
