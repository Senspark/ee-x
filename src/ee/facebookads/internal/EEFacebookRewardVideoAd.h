//
//  EEFacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

@protocol EEIMessageBridge;

@interface EEFacebookRewardVideoAd : NSObject

- (id _Nonnull)initWithBridge:(id<EEIMessageBridge> _Nonnull)bridge
                  placementId:(NSString* _Nonnull)placementId;

- (void)destroy;

- (BOOL)hasRewardVideo;

- (void)loadRewardedVideo;

- (BOOL)showRewardVideo;

@end
