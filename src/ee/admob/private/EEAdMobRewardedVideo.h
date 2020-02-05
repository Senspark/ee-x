//
//  EEAdMobInterstitialAd.h
//  ee_x
//
//  Created by KietLe on 12/04/19.
//
//

#import <ee/ads/internal/EEIInterstitialAd.h>

@protocol EEIMessageBridge;

@interface EEAdMobRewardedVideo : NSObject

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId;

- (void)destroy;

@end
