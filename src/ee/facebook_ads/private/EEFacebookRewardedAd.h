//
//  EEFacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

@protocol EEIMessageBridge;

@interface EEFacebookRewardedAd : NSObject

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                         adId:(NSString* _Nonnull)adId;

- (void)destroy;

@end
