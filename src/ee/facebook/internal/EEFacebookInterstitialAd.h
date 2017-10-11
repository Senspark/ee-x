//
//  EEFacebookInterstitialAd.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import <Foundation/NSObject.h>

@interface EEFacebookInterstitialAd : NSObject

- (id _Nonnull)initWithPlacementId:(NSString* _Nonnull)placementId;

- (BOOL)isLoaded;
- (void)load;
- (void)show;

@end
