//
//  EEFacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import "ee/ads/internal/EEInterstitialAdInterface.h"

@interface EEFacebookInterstitialAd : NSObject <EEInterstitialAdInterface>

- (id _Nonnull)initWithPlacementId:(NSString* _Nonnull)placementId;

- (void)destroy;

@end
