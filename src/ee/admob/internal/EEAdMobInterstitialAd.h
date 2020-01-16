//
//  EEAdMobInterstitialAd.h
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#import <ee/ads/EEIInterstitialAd.h>

@protocol EEIMessageBridge;

@interface EEAdMobInterstitialAd : NSObject <EEIInterstitialAd>

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId;

- (void)destroy;

@end
