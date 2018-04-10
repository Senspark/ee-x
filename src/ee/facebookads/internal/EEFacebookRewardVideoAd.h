//
//  EEFacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

@interface EEFacebookRewardVideoAd : NSObject 

- (id _Nonnull)initWithPlacementId:(NSString* _Nonnull)placementId;

- (void)destroy;

-(BOOL) hasRewardVideo;

-(void) loadRewardedVideo;

-(BOOL) showRewardVideo;
@end
