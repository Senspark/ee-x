//
//  EEALovinAds.h
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

@protocol EEIPlugin;

@interface EEAppLovin : NSObject <EEIPlugin>

- (void)destroy;

- (void)initialize:(NSString* _Nonnull)key;

- (void)setTestAdsEnabled:(BOOL)enabled;
- (void)setVerboseLogging:(BOOL)enabled;
- (void)setMuted:(BOOL)enabled;

- (BOOL)hasInterstitialAd;
- (void)loadInterstitialAd;
- (void)showInterstitialAd;

- (BOOL)hasRewardedAd;
- (void)loadRewardedAd;
- (void)showRewardedAd;

@end
