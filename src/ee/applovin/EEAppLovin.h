//
//  EEALovinAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#import <Foundation/Foundation.h>

@interface EEAppLovin : NSObject

- (void)initialize:(NSString* _Nonnull)key;

- (void)setTestAdsEnabled:(BOOL)enabled;
- (void)setVerboseLogging:(BOOL)enabled;
- (void)setMuted:(BOOL)enabled;

- (BOOL)hasInterstitialAd;
- (void)showInterstitialAd;

- (void)loadRewardedVideo;
- (BOOL)hasRewardedVideo;
- (void)showRewardedVideo;

@end
