//
//  EEAdMobInterstitialAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import "ee/ads/internal/EEIInterstitialAd.h"

@protocol EEIMessageBridge;

@interface EEAdMobInterstitialAd : NSObject <EEIInterstitialAd>

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge>)bridge
                         adId:(NSString* _Nonnull)adId
                  testDevices:(NSArray<NSString*>* _Nullable)testDevices;

- (void)destroy;

@end
