//
//  EEFacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import "ee/ads/internal/EEIInterstitialAd.h"

@interface EEFacebookInterstitialAd : NSObject <EEIInterstitialAd>

- (id _Nonnull)initWithPlacementId:(NSString* _Nonnull)placementId;

- (void)destroy;

@end
