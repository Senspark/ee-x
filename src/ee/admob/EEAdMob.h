//
//  EEAdMob.hpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#import <Foundation/NSObject.h>

@interface EEAdMob : NSObject

- (BOOL)createBannerAd:(NSString* _Nonnull)adId size:(GADAdSize)size;
- (BOOL)destroyBannerAd:(NSString* _Nonnull)adId;

- (BOOL)createInterstitialAd:(NSString* _Nonnull)adId;
- (BOOL)destroyInterstitialAd:(NSString* _Nonnull)adId;

- (void)loadRewardedVideo:(NSString* _Nonnull)adId;
- (bool)hasRewardedVideo;
- (void)showRewardedVideo;

@end
