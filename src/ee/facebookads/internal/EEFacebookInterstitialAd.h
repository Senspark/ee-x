//
//  EEFacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import "ee/ads/internal/EEIInterstitialAd.h"

@class EEMessageBridge;

@interface EEFacebookInterstitialAd : NSObject <EEIInterstitialAd>

- (id _Nonnull)initWithBridge:(EEMessageBridge* _Nonnull)bridge
                  placementId:(NSString* _Nonnull)placementId;

- (void)destroy;

@end
