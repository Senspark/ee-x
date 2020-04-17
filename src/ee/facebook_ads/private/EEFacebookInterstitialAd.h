//
//  EEFacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import <ee/ads/internal/EEIInterstitialAd.h>

@protocol EEIMessageBridge;

@interface EEFacebookInterstitialAd : NSObject <EEIInterstitialAd>

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId;

- (void)destroy;

@end
